// Aqsis
// Copyright (C) 1997 - 2001, Paul C. Gregory
//
// Contact: pgregory@aqsis.org
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifdef AQSIS_SYSTEM_WIN32
#include <io.h>
#endif

#include <cstring>

#include <Partio.h>

#include "shaderexecenv.h"


#include <aqsis/util/autobuffer.h>
#include <aqsis/util/logging.h>

#include <OpenEXR/ImathVec.h>


namespace Aqsis {


using Imath::V3f;

// Utils for bake3d() shadeop.
namespace {
struct UserVar
{
    IqShaderData* value;
    EqVariableType type;
    Partio::ParticleAttribute attr;

    UserVar(IqShaderData* value, EqVariableType type,
             const Partio::ParticleAttribute& attr)
        : value(value), type(type), attr(attr) {}
    UserVar(IqShaderData* value, EqVariableType type)
        : value(value), type(type) {}
};
}



//------------------------------------------------------------------------------

static void releasePartioFile(Partio::ParticlesInfo* file) {
	if (file)
		file->release();
}

namespace {
/// A cache for open point cloud bake files for texture3d().
class Texture3dCache
{
    public:


        /// Find a point cloud with the given name, or open it from file.
        Partio::ParticlesData* find(const std::string& fileName)
        {
            Partio::ParticlesDataMutable* pointFile = 0;
            FileMap::iterator ptcIter = m_files.find(fileName);
            if(ptcIter == m_files.end())
            {
                // Create new bake file & insert into map.
                pointFile = Partio::read(fileName.c_str());
                m_files[fileName].reset(pointFile, releasePartioFile);
                if(pointFile)
                {
                    // Sort file so that it can be looked up efficiently.
                    pointFile->sort();
                    // TODO: Ensure that radius, position & normal attributes
                    // are present.
                }
                else
                {
                    Aqsis::log() << error
                        << "texture3d: Could not open point cloud \"" << fileName
                        << "\" for reading\n";
                }
            }
            else
                pointFile = ptcIter->second.get();
            return pointFile;
        }

        /// Flush all files from the cache.
        void clear()
        {
            m_files.clear();
        }

    private:
        typedef std::map<std::string, boost::shared_ptr<Partio::ParticlesDataMutable> > FileMap;
        FileMap m_files;
};
}


//------------------------------------------------------------------------------
// Texture3d stuff

// Parse and validate the varargs part of the texture3d argument list.
//
// nargs and args specify the varargs list.
static bool parseTexture3dVarargs(int nargs, IqShaderData** args,
                                  const Partio::ParticlesData* pointFile,
                                  CqString& coordSystem,
                                  std::vector<UserVar>& userVars)
{
    userVars.reserve(nargs/2);
    CqString paramName;
    for(int i = 0; i+1 < nargs; i+=2)
    {
        if(args[i]->Type() != type_string)
        {
            Aqsis::log() << error
                << "unexpected non-string for parameter name in texture3d()\n";
            return false;
        }
        args[i]->GetString(paramName);
        IqShaderData* paramValue = args[i+1];
        EqVariableType paramType = paramValue->Type();
        // Parameters with special meanings may be present in the varargs
        // list, and shouldn't be looked up in the file:
        if(paramName == "coordsystem" && paramType == type_string)
            paramValue->GetString(coordSystem);
        else if(paramName == "filterradius")
            ; // For brick maps; ignored for now
        else if(paramName == "filterscale")
            ; // For brick maps; ignored for now
        else if(paramName == "maxdepth")
            ; // For brick maps; ignored for now
        else
        {
            // If none of the above special cases, we have a user-defined
            // variable.  Look it up in the point file, and check whether the
            // type corresponds with the provided shader variable.
            Partio::ParticleAttribute attr;
            pointFile->attributeInfo(paramName.c_str(), attr);
            if(attr.type != Partio::FLOAT && attr.type != Partio::VECTOR)
            {
                Aqsis::log() << warning
                    << "texture3d: Can't load non-float data \""
                    << paramName << "\"\n";
                continue;
            }
            int desiredCount = 0;
            switch(paramType)
            {
                case type_float:  desiredCount = 1;  break;
                case type_point:  desiredCount = 3;  break;
                case type_color:  desiredCount = 3;  break;
                case type_normal: desiredCount = 3;  break;
                case type_vector: desiredCount = 3;  break;
                case type_matrix: desiredCount = 16; break;
                default:
                    Aqsis::log() << warning
                        << "texture3d: Can't load non float-based argument \""
                        << paramName << "\"\n";
                    continue;
            }
            if(desiredCount != attr.count)
            {
                Aqsis::log() << warning
                    << "texture3d: variable \"" << paramName
                    << "\" mismatched in point file\n";
                continue;
            }
            userVars.push_back(UserVar(paramValue, paramType, attr));
        }
    }
    return userVars.size() > 0;
}

// TODO: Make non-global
static Texture3dCache g_texture3dCloudCache;

void flushTexture3dCache()
{
    g_texture3dCloudCache.clear();
}

/** \brief Shadeops "texture3d" to restore any parameter from one pointcloud file refer.
 *  \param ptc the name of the pointcloud file
 *  \param position
 *  \param normal
 *  \result result 0 or 1
 *  \param pShader shaderexecenv
 *  \param cParams number of remaining user parameters.
 *  \param aqParams list of user parameters (to save to ptc)
 */
void CqShaderExecEnv::SO_texture3d(IqShaderData* ptc,
                                   IqShaderData* position,
                                   IqShaderData* normal,
                                   IqShaderData* Result,
                                   IqShader* pShader,
                                   TqInt cParams, IqShaderData** apParams )
{
    const CqBitVector& RS = RunningState();
    CqString ptcName;
    ptc->GetString(ptcName);

    Partio::ParticlesData* pointFile = g_texture3dCloudCache.find(ptcName);
    bool varying = position->Class() == class_varying ||
                   normal->Class() == class_varying ||
                   Result->Class() == class_varying;
    int npoints = varying ? shadingPointCount() : 1;

    CqString coordSystem = "world";
    std::vector<UserVar> userVars;

    if(!pointFile || !parseTexture3dVarargs(cParams, apParams,
                                            pointFile, coordSystem, userVars))
    {
        // Error - no point file or no arguments to look up: set result to 0
        // and return.
        for(int igrid = 0; igrid < npoints; ++igrid)
            if(!varying || RS.Value(igrid))
                Result->SetFloat(0, igrid);
        return;
    }

    /// Compute transformations
    CqMatrix positionTrans;
    getRenderContext()->matSpaceToSpace("current", coordSystem.c_str(),
                                        pShader->getTransform(),
                                        pTransform().get(), 0, positionTrans);
    CqMatrix normalTrans = normalTransform(positionTrans);

    // Grab the standard attributes for computing filter weights
    Partio::ParticleAttribute positionAttr, normalAttr, radiusAttr;
    pointFile->attributeInfo("position", positionAttr);
    pointFile->attributeInfo("normal", normalAttr);
    pointFile->attributeInfo("radius", radiusAttr);

    for(int igrid = 0; igrid < npoints; ++igrid)
    {
        if(varying && !RS.Value(igrid))
            continue;
        CqVector3D cqP, cqN;
        position->GetPoint(cqP, igrid);
        cqP = positionTrans*cqP;
        normal->GetNormal(cqN, igrid);
        cqN = normalTrans*cqN;

        // Using the four nearest neighbours for filtering is roughly the
        // minimum we can get away with for a surface made out of
        // quadrilaterals.  This isn't exactly perfect near edges but it seems
        // to be good enough.
        //
        // Since the lookups happen without prefiltering, no effort is made to
        // avoid aliasing by using a filter radius based on the size of the
        // current shading element.
        const int nfilter = 4;
        Partio::ParticleIndex indices[nfilter];
        float distSquared[nfilter];
        float maxRadius2 = 0;
        V3f P(cqP.x(), cqP.y(), cqP.z());
        // The reinterpret_casts are ugly here of course, but V3f is basically
        // a POD type, so they work ok.
        int pointsFound = pointFile->findNPoints(reinterpret_cast<float*>(&P),
                                                 nfilter, FLT_MAX, indices,
                                                 distSquared, &maxRadius2);
        if(pointsFound < nfilter)
        {
            Result->SetFloat(0.0f, igrid);
            Aqsis::log() << error << "Not enough points found to filter!";
        }
        // Read position, normal and radius
        V3f foundP[nfilter];
        pointFile->dataAsFloat(positionAttr, nfilter, indices, false,
                               reinterpret_cast<float*>(foundP));
        V3f foundN[nfilter];
        pointFile->dataAsFloat(normalAttr, nfilter, indices, false,
                               reinterpret_cast<float*>(foundN));
        float foundRadius[nfilter];
        pointFile->dataAsFloat(radiusAttr, nfilter, indices, false, foundRadius);

        // Compute filter weights for nearby points.  inverseWidthSquared
        // decides the blurryness of the gaussian filter.  The value was chosen
        // by eyeballing the results of various widths.  As usual it's a trade
        // off: Too small a value will be too blurry, (and artifacty if nfilter
        // is 4); too large a value and it ends up looking like nearest
        // neighbour filtering.
        const float inverseWidthSquared = 1.3f;
        float weights[nfilter];
        float totWeight = 0;
        V3f N(cqN.x(), cqN.y(), cqN.z());
        for(int i = 0; i < nfilter; ++i)
        {
            // The weights depend on how well the normals are aligned, and the
            // distance between the current shading point and the points found
            // in the point cloud.
            float wN = std::max(0.0f, N.dot(foundN[i]));
            // TODO: Speed this up using a lookup table for exp?
            float wP = std::exp(-inverseWidthSquared * distSquared[i]
                                / (foundRadius[i]*foundRadius[i]));
            // Clamping to a minimum of 1e-7 means the weights don't quite
            // become zero as distSquared becomes large, so texture lookups
            // even far from any point in the cloud return something nonzero.
            //
            // Not sure if this is a good idea or not, so commented out for now
            // wP = std::max(1e-7f, wP);
            float w = wN*wP;
            weights[i] = w;
            totWeight += w;
        }
        // Normalize the weights
        float renorm = totWeight != 0 ? 1/totWeight : 0;
        for(int i = 0; i < nfilter; ++i)
            weights[i] *= renorm;

        for(std::vector<UserVar>::const_iterator var = userVars.begin();
            var != userVars.end(); ++var)
        {
            // Read and filter each piece of user-defined data
            float varData[16*nfilter];
            pointFile->dataAsFloat(var->attr, nfilter, indices, false, varData);
            int varSize = var->attr.count;
            float accum[16];
            for(int c = 0; c < varSize; ++c)
                accum[c] = 0;
            for(int i = 0; i < nfilter; ++i)
                for(int c = 0; c < varSize; ++c)
                    accum[c] += weights[i] * varData[i*varSize + c];
            // Ah, if only we could get at the raw floats stored by
            // IqShaderData, we wouldn't need this switch
            switch(var->type)
            {
                case type_float:
                    var->value->SetFloat(accum[0], igrid);
                    break;
                case type_color:
                    var->value->SetColor(CqColor(accum[0], accum[1],
                                                 accum[2]), igrid);
                    break;
                case type_point:
                    var->value->SetPoint(CqVector3D(accum[0], accum[1],
                                                    accum[2]), igrid);
                    break;
                case type_normal:
                    var->value->SetNormal(CqVector3D(accum[0], accum[1],
                                                     accum[2]), igrid);
                    break;
                case type_vector:
                    var->value->SetVector(CqVector3D(accum[0], accum[1],
                                                     accum[2]), igrid);
                    break;
                case type_matrix:
                    var->value->SetMatrix(CqMatrix(accum), igrid);
                    break;
                default: assert(0 && "unknown type");
            }
        }
        // Return success for this point
        Result->SetFloat(1.0f, igrid);
    }
}

//---------------------------------------------------------------------
}
