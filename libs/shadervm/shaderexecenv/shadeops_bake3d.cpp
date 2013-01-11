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

namespace Aqsis
{

using Imath::V3f;

// Utils for bake3d() shadeop.
namespace {
struct UserVar
{
    IqShaderData* value;
    EqVariableType type;
    Partio::ParticleAttribute attr;
    int length;

    UserVar(IqShaderData* value, EqVariableType type,
             const Partio::ParticleAttribute& attr)
        : value(value), type(type), attr(attr), length(1) {}
    UserVar(IqShaderData* value, EqVariableType type,
             const Partio::ParticleAttribute& attr, int length)
        : value(value), type(type), attr(attr), length(length) {}
    UserVar(IqShaderData* value, EqVariableType type)
        : value(value), type(type), attr(), length(1) {}
    UserVar(IqShaderData* value, EqVariableType type, int length)
        : value(value), type(type), attr(), length(length) {}
};
}

/// Extract float data from variables to be baked.
///
/// \param out - all output variables are squashed together into this array.
///              The ordering is P[3] N[3] userdata[...]
/// \param igrid - grid index at which to retrieve the data
/// \param position - position array
/// \param normal - normal array
/// \param bakeVars - array of shader data and associated types
/// \param nBakeVars - length of bakeVars array.
static void extractUserVars(float* out, int igrid, IqShaderData* position,
                            IqShaderData* normal, UserVar* bakeVars,
                            int nBakeVars)
{
    // Temp vars for extracting data from IqShaderData
    TqFloat f;
    CqVector3D p;
    CqColor c;
    CqMatrix m;


    // Extract position and normal; always required
    position->GetPoint(p, igrid);
    *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
    normal->GetNormal(p, igrid);
    *out++ = p.x(); *out++ = p.y(); *out++ = p.z();

    // Get all user-defined parameters and assemble them together into array
    // of floats to pass to Ptc API.
    for(int i = 0; i < nBakeVars; ++i)
    {
        const UserVar& var = bakeVars[i];
        switch(var.type)
        {
            case type_float:

            	if (var.value->isArray()) {
            		for (int i=0; i < var.value->ArrayLength(); i++){
            			var.value->ArrayEntry(i)->GetFloat(f, igrid);
            			*out++ = f;
            		}
            	} else {
            		var.value->GetFloat(f, igrid);
            		*out++ = f;
            	}
                break;

            case type_point:

            	if (var.value->isArray()) {
            		for (int i; i < var.value->ArrayLength(); i++) {
						var.value->ArrayEntry(igrid)->GetPoint(p, i);
		                *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
					}
            	} else {
            		var.value->GetPoint(p, igrid);
                    *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
            	}
                break;

            case type_normal:

            	if (var.value->isArray()) {
            		for (int i; i < var.value->ArrayLength(); i++) {
						var.value->ArrayEntry(igrid)->GetNormal(p, i);
		                *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
					}
            	} else {
            		var.value->GetNormal(p, igrid);
                    *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
            	}
                break;

            case type_vector:

            	if (var.value->isArray()) {
            		for (int i; i < var.value->ArrayLength(); i++) {
						var.value->ArrayEntry(igrid)->GetVector(p, i);
		                *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
					}
            	} else {
            		var.value->GetVector(p, igrid);
                    *out++ = p.x(); *out++ = p.y(); *out++ = p.z();
            	}
                break;

            case type_color:

            	if (var.value->isArray()) {
            		for (int i; i < var.value->ArrayLength(); i++) {
						var.value->ArrayEntry(igrid)->GetColor(c, i);
		                *out++ = c.r(); *out++ = c.g(); *out++ = c.b();
					}
            	} else {
            		var.value->GetColor(c, igrid);
                    *out++ = c.r(); *out++ = c.g(); *out++ = c.b();
            	}
                break;

            case type_matrix:

            	if (var.value->isArray()) {
            		for (int i; i < var.value->ArrayLength(); i++) {
						var.value->ArrayEntry(igrid)->GetMatrix(m, i);
		                for(int k = 0; k < 4; ++k)
		                for(int h = 0; h < 4; ++h)
		                    *out++ = m[k][h];
					}
            	} else {
            		var.value->GetMatrix(m, igrid);
                    for(int k = 0; k < 4; ++k)
                    for(int h = 0; h < 4; ++h)
                        *out++ = m[k][h];
            	}
                break;

            default:
                assert(0 && "unexpected type");
                break;
        }
    }
}

//------------------------------------------------------------------------------

static void releasePartioFile(Partio::ParticlesInfo* file)
{
    if(file) file->release();
}

namespace {
/// A cache for open point cloud bake files for bake3d().
class Bake3dCache
{
    public:
        /// Find or create a point cloud with the given name.
        ///
        /// The standard attributes; position, normal, and radius are added on
        /// creation.
        Partio::ParticlesDataMutable* find(const std::string& fileName)
        {
            Partio::ParticlesDataMutable* pointFile = 0;
            FileMap::iterator ptcIter = m_files.find(fileName);
            if(ptcIter == m_files.end())
            {
                // Create new bake file & insert into map.
                pointFile = Partio::create();
                m_files[fileName].reset(pointFile, releasePartioFile);
                if(pointFile)
                {
                    // Add default attributes
                    pointFile->addAttribute("position", Partio::VECTOR, 3);
                    pointFile->addAttribute("normal", Partio::VECTOR, 3);
                    pointFile->addAttribute("radius", Partio::FLOAT, 1);
                }
                else
                {
                    Aqsis::log() << error
                        << "bake3d: Could not open point cloud \"" << fileName
                        << "\" for writing\n";
                }
            }
            else
                pointFile = ptcIter->second.get();
            return pointFile;
        }

        /// Flush all files to disk and clear the cache
        void flush()
        {
            for(FileMap::iterator i = m_files.begin(); i != m_files.end(); ++i)
                Partio::write(i->first.c_str(), *i->second);
            m_files.clear();
        }

    private:
        typedef std::map<std::string, boost::shared_ptr<Partio::ParticlesDataMutable> > FileMap;
        FileMap m_files;
};
}

// TODO: Make non-global
static Bake3dCache g_bakeCloudCache;

void flushBakeCache()
{
    g_bakeCloudCache.flush();
}

//------------------------------------------------------------------------------
/// Shadeop to bake vertices to point cloud
///
/// bake3d(fileName, P, N, format, ...)
///
/// \param ptc      - the name of the pointcloud file
/// \param channels - not used.  For PRMan compatibility.)
/// \param position - vertex position
/// \param normal   - normal at vertex
/// \result result  - 0 or 1 for failure or success
/// \param pShader  - unused
/// \param cParams  - number of extra user parameters.
/// \param apParams - list of extra parameters to control output or save to ptc.
///
void CqShaderExecEnv::SO_bake3d( IqShaderData* ptc,
                                 IqShaderData* channels,
                                 IqShaderData* position,
                                 IqShaderData* normal,
                                 IqShaderData* Result,
                                 IqShader* pShader,
                                 TqInt cParams, IqShaderData** apParams )
{
    const CqBitVector& RS = RunningState();
    CqString ptcName;
    ptc->GetString(ptcName);
    // Find point cloud in cache, or create it if it doesn't exist.
    Partio::ParticlesDataMutable* pointFile = g_bakeCloudCache.find(ptcName);
    bool varying = position->Class() == class_varying ||
                   normal->Class() == class_varying ||
                   Result->Class() == class_varying;
    if(!pointFile)
    {
        // Error; set result to false and return.
        int npoints = varying ? shadingPointCount() : 1;
        for(int igrid = 0; igrid < npoints; ++igrid)
            if(!varying || RS.Value(igrid))
                Result->SetFloat(0.0f, igrid);
        return;
    }
    // Optional output control variables
    bool interpolate = false;
    const IqShaderData* radius = 0;
    const IqShaderData* radiusScale = 0;
    CqString coordSystem = "world";
    // P, N and r output attributes are always present
    Partio::ParticleAttribute positionAttr, normalAttr, radiusAttr;
    pointFile->attributeInfo("position", positionAttr);
    pointFile->attributeInfo("normal", normalAttr);
    pointFile->attributeInfo("radius", radiusAttr);
    // Extract list of user-specified output vars from arguments
    std::vector<UserVar> bakeVars;
    bakeVars.reserve(cParams/2);
    CqString paramName;
    // Number of output floats.  Start with space for position and normal data.
    int nOutFloats = 6;
    for(int i = 0; i+1 < cParams; i+=2)
    {
        if(apParams[i]->Type() == type_string)
        {
            apParams[i]->GetString(paramName);
            IqShaderData* paramValue = apParams[i+1];
            EqVariableType paramType = paramValue->Type();
            // Parameters with special meanings may be present in the varargs
            // list, but shouldn't be saved to the output file, these include:
            if(paramName == "interpolate" && paramType == type_float)
                paramValue->GetBool(interpolate);
            else if(paramName == "radius" && paramType == type_float)
                radius = paramValue;
            else if(paramName == "radiusscale" && paramType == type_float)
                radiusScale = paramValue;
            else if(paramName == "coordsystem" && paramType == type_string)
                paramValue->GetString(coordSystem);
            else
            {
                // If none of the above special cases, we have an output
                // variable which should be saved to the file.
                int count = 0;
                Partio::ParticleAttributeType parType = Partio::FLOAT;

                int length = 1;
                if (paramValue->isArray()) {
                	length = paramValue->ArrayLength();
                }

                switch(paramType)
                {
                    case type_float:  count = 1*length;  parType = Partio::FLOAT;  break;
                    case type_point:  count = 3*length;  parType = Partio::VECTOR; break;
                    case type_color:  count = 3*length;  parType = Partio::FLOAT;  break;
                    case type_normal: count = 3*length;  parType = Partio::VECTOR; break;
                    case type_vector: count = 3*length;  parType = Partio::VECTOR; break;
                    case type_matrix: count = 16*length; parType = Partio::FLOAT;  break;
                    default:
                        Aqsis::log() << warning
                            << "bake3d: Can't save non-float argument \""
                            << paramName << "\"\n";
                        continue;
                }
                bakeVars.push_back(UserVar(paramValue, paramType, count));
                // Find the named attribute in the point file, or create it if
                // it doesn't exist.
                UserVar& var = bakeVars.back();
                if(pointFile->attributeInfo(paramName.c_str(), var.attr))
                {
                    if(var.attr.count != count)
                    {
                        Aqsis::log() << warning
                            << "bake3d: can't bake variable \"" << paramName
                            << "\"; previously baked with different type\n";
                        bakeVars.pop_back();
                    }
                }
                else {
                	Aqsis::log() << warning << "bake3d: Baking attribute " << paramName.c_str()
                			<< " of type " << parType << " ("<<count<<")\n";

                	var.attr = pointFile->addAttribute(paramName.c_str(),parType, count);
                }

                nOutFloats += count;
            }
        }
        else
            Aqsis::log() << "unexpected non-string for parameter name "
                            "in bake3d()\n";
    }

    /// Compute transformations
    CqMatrix positionTrans;
    getRenderContext()->matSpaceToSpace("current", coordSystem.c_str(),
                                        pShader->getTransform(),
                                        pTransform().get(), 0, positionTrans);
    CqMatrix normalTrans = normalTransform(positionTrans);

    std::vector<TqFloat> allDataVec(interpolate ? 2*nOutFloats : nOutFloats);
    TqFloat* allData = &allDataVec[0];

    // Number of vertices in the grid
    int uSize = m_uGridRes+1;
    int vSize = m_vGridRes+1;

    TqUint igrid = 0;
    do
    {
        if(RS.Value( igrid ) )
        {
            int iu = 0, iv = 0;
            if(interpolate)
            {
                // Get micropoly position on 2D grid.
                // TODO: What if the grid is 1D or 0D?
                iv = igrid / uSize;
                iu = igrid - iv*uSize;
                // Check whether we're off the edge (number of polys in each
                // direction is one less than number of verts)
                if(iv == vSize - 1 || iu == uSize - 1)
                    continue;
            }

            // Extract all baking variables into allData.
            extractUserVars(allData, igrid, position, normal,
                            &bakeVars[0], bakeVars.size());

            // Get radius if it's avaliable, otherwise compute automatically
            // below.
            float radiusVal = 0;
            if(radius)
                radius->GetFloat(radiusVal, igrid);

            if(interpolate)
            {
                int interpIndices[3] = {
                    iv*uSize       + iu + 1,
                    (iv + 1)*uSize + iu,
                    (iv + 1)*uSize + iu + 1
                };
                float* tmpData = allData + nOutFloats;
                float* outData = allData;
                CqVector3D P[4]; // current micropoly vertex positions.
                P[0] = CqVector3D(outData);
                // Extract data from the three other verts on the current
                // micropolygon & merge into outData.
                for(int i = 0; i < 3; ++i)
                {
                    extractUserVars(tmpData, interpIndices[i], position, normal,
                                    &bakeVars[0], bakeVars.size());
                    for(int j = 0; j < nOutFloats; ++j)
                        outData[j] += tmpData[j];
                    P[i+1] = CqVector3D(tmpData);
                }
                // normalize averages
                for(int j = 0; j < nOutFloats; ++j)
                    outData[j] *= 0.25f;
                if(!radius)
                {
                    CqVector3D Pmid = CqVector3D(outData);
                    // Compute radius using vertex positions.  Radius is the
                    // maximum distance from the centre of the micropolygon to
                    // a vertex.
                    radiusVal = (Pmid - P[0]).Magnitude2();
                    for(int i = 1; i < 4; ++i)
                        radiusVal = std::max(radiusVal,
                                             (Pmid - P[i]).Magnitude2());
                    radiusVal = std::sqrt(radiusVal);
                }
            }
            else
            {
                if(!radius)
                {
                    // Extract radius, non-interpolation case.
                    CqVector3D e1 = diffU<CqVector3D>(position, igrid);
                    CqVector3D e2 = diffV<CqVector3D>(position, igrid);
                    // Distances from current vertex to diagonal neighbours.
                    float d1 = (e1 + e2).Magnitude2();
                    float d2 = (e1 - e2).Magnitude2();
                    // Choose distance to furtherest diagonal neighbour so
                    // that the disks just overlap to produce a surface
                    // without holes.  The factor of 0.5 gives the radius
                    // rather than diameter.
                    radiusVal = 0.5f*std::sqrt(std::max(d1, d2));
                }
            }

            // Scale radius if desired.
            if(radiusScale)
            {
                float scale = 1;
                radiusScale->GetFloat(scale, igrid);
                radiusVal *= scale;
            }

            // Save current point data to the point file
            float* d = &allData[0];
            Partio::ParticleIndex ptIdx = pointFile->addParticle();
            // Save out standard attributes
            float* P = pointFile->dataWrite<float>(positionAttr, ptIdx);
            float* N = pointFile->dataWrite<float>(normalAttr, ptIdx);
            float* r = pointFile->dataWrite<float>(radiusAttr, ptIdx);
            CqVector3D cqP = positionTrans * CqVector3D(d[0], d[1], d[2]); d += 3;
            P[0] = cqP.x(); P[1] = cqP.y(); P[2] = cqP.z();
            CqVector3D cqN = normalTrans * CqVector3D(d[0], d[1], d[2]); d += 3;
            N[0] = cqN.x(); N[1] = cqN.y(); N[2] = cqN.z();
            r[0] = radiusVal;
            // Save out user-defined attributes
            for(int i = 0, iend = bakeVars.size(); i < iend; ++i)
            {
                UserVar& var = bakeVars[i];
                float* out = pointFile->dataWrite<float>(var.attr, ptIdx);

               	for(int j = 0; j < var.attr.count; ++j) {
//               		Aqsis::log() << warning << "igrid " << igrid << " write float "<< *d
//               		               << " for attr: "<<var.attr.name<< std::endl;
               	    out[j] = *d++;
               	}
            }
            Result->SetFloat(1, igrid);
        }

    }
    while( ( ++igrid < shadingPointCount() ) && varying);
}



} // namespace Aqsis
