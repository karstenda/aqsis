// AUTHOR: KARSTEN DAEMEN
// THIS CLASS

#ifdef AQSIS_SYSTEM_WIN32
#include <io.h>
#endif


#include	<stdio.h>
#include	<iostream>
#include	<string>
#include	<cstring>
#include	<Partio.h>
#include	<OpenEXR/ImathVec.h>


#include	<aqsis/util/autobuffer.h>
#include	<aqsis/util/logging.h>
#include	<aqsis/math/math.h>
#include	<aqsis/core/ilightsource.h>

#include	"../../pointrender/MicroBuf.h"
#include	"../../pointrender/PointOctreeCache.h"
#include	"../../pointrender/RadiosityIntegrator.h"
#include	"../../pointrender/microbuf_proj_func.h"

#include	"../../pointrender/nondiffuse/NonDiffusePointCloud.hpp"
#include	"../../pointrender/nondiffuse/NonDiffusePointCloudCache.hpp"
#include	"shaderexecenv.h"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

// Cache for previously loaded point clouds
static PointOctreeCache g_diffusePtcCache;
static NonDiffusePointCloudCache g_nonDiffusePtcCache;

/**
 * Shadeop to bake non diffuse point cloud from diffuse point cloud.
 *
 *
 * \param ptc		- The name of the pointcloud file.
 * \param channels	- Not used, is here for PRMan compatibility
 * \param position	- Vertex positions (IqShaderData* is organised as a list of	vectors).
 * \param normal 	- Normals (IqShaderData* is organised as a list of vectors).
 * \param result 	- 0 or 1, depending on failure or success
 * \param pShader 	- Unused.
 * \param cParams	- Number of extra user parameters.
 * \param apParams	- List of extra parameters to control. Parameter name is always
 * 					followed by the value.
 *
 */

void CqShaderExecEnv::SO_bake3d_brdf(	IqShaderData* channels,
											IqShaderData* P,
											IqShaderData* N,
											IqShaderData* result,
											IqShader* pShader,
											TqInt cParams,
											IqShaderData** apParams) {

	// Check if there's a RenderContext.
	// (is this function called during rendering?)
	if (!getRenderContext()) {
		return;
	}

	/*
	 * Variables
	 */

	// The pointer to the octree containing the diffuse surphels.
	PointOctree* diffusePtc = 0;
	// The pointer to the cloud containging the nondiffuse surphels.
	NonDiffusePointCloud* nonDiffusePtc = 0;
	// Resolution of the microbuffer face.
	int faceRes = 10;
	// The maximum solid angle to use during the octree traversal.
	float maxSolidAngle = 0.03;
	// The cone angle of each point (whole hemisphere default).
	float coneAngle = M_PI_2;
	// The bias to introduce.
	float bias = 0;
	// Default coordinate system to use
	CqString coordSystem = "world";
	// Phong exponent
	int phong = -1;
	// fileName Diffuse pointcloud
	CqString fileNameDiffusePtc;
	//fileName NonDiffuse pointcloud
	CqString fileNameNonDiffusePtc;



	/*
	 * Parsing these parameters ...
	 */
	CqString paramName;
	for (int i = 0; i < cParams; i += 2) {
		apParams[i]->GetString(paramName, 0);
		IqShaderData* paramValue = apParams[i + 1];
		if (paramName == "coneangle") {
			if (paramValue->Type() == type_float)
				paramValue->GetFloat(coneAngle);
		} else if (paramName == "diffuse_ptc") {
			if (paramValue->Type() == type_string) {
				paramValue->GetString(fileNameDiffusePtc, 0);
			}
		} else if (paramName == "nondiffuse_ptc") {
			if (paramValue->Type() == type_string) {
			paramValue->GetString(fileNameNonDiffusePtc, 0);
			}
		} else if (paramName == "coordsystem") {
			if (paramValue->Type() == type_string)
				paramValue->GetString(coordSystem);
		} else if (paramName == "maxsolidangle") {
			if (paramValue->Type() == type_float)
				paramValue->GetFloat(maxSolidAngle);
		} else if (paramName == "bias") {
			if (paramValue->Type() == type_float)
				paramValue->GetFloat(bias);
		} else if (paramName == "microbufres") {
			if (paramValue->Type() == type_float) {
				float res = 10;
				paramValue->GetFloat(res);
				faceRes = std::max(1, static_cast<int> (res));
			}
		} else if (paramName == "phong") {
			if (paramValue->Type() == type_float) {
				float exponent = -1;
				paramValue->GetFloat(exponent);
				phong = std::max(0, static_cast<int> (exponent));
			}
		}
	}

	// Load the point clouds.
	if (!fileNameDiffusePtc.empty()) {
		diffusePtc = g_diffusePtcCache.find(fileNameDiffusePtc);
	}
	if (!fileNameNonDiffusePtc.empty()) {
		nonDiffusePtc = g_nonDiffusePtcCache.findOrCreate(fileNameNonDiffusePtc,faceRes,5);;
	}

	// TODO: Debug statement
//	Aqsis::log() << warning << "Phong exponent: " << phong << "\n";

	// Compute current transform to appropriate space.
	// During rasterisation, the coordinates are not real world coordinates.
	CqMatrix positionTrans;
	getRenderContext()->matSpaceToSpace("current", coordSystem.c_str(),
			pShader->getTransform(), pTransform().get(), 0, positionTrans);
	CqMatrix normalTrans = normalTransform(positionTrans);

	// Number of vertices in the u-direction of this shading grid.
	int uSize = m_uGridRes + 1;

	// Does the shading vary over the grid?
	// See Renderman varying attribute for primitives.
	bool varying = result->Class() == class_varying;

	// Still no clue about what is in this vector ...
	// Has something to do with the SIMD stack of the shadervm?
	const CqBitVector& RS = RunningState();

	if (diffusePtc) {

		// How many points do have to be baked?
		int npoints = 1;
		if (varying) {
			npoints = shadingPointCount();
		}

		// openMP macro to indicate pieces of code that can be run in parallel.
#pragma omp parallel
		{
			// Define the integrator to hold the microbuffer.
			RadiosityIntegrator integrator(faceRes);

			// openMP macro to indicate pieces of code that can be run in parallel.
#pragma omp for

			// For every shading point in this shading grid do ...
			for (int igrid = 0; igrid < npoints; ++igrid) {
				if (!varying || RS.Value(igrid)) {

					// Initiate the position of the shading point. Based on the
					// position in the grid.
					//
					// MARK: RiPoints are not organised as a 2D grid. They will
					//		 therefore not be rendered correctly.
					CqVector3D Pval;
					int v = igrid / uSize;
					int u = igrid - v * uSize;
					float uinterp = 0;
					float vinterp = 0;
					P->GetVector(Pval, igrid);

					// Calculate the position and the normal of the shadingpoint
					CqVector3D Nval;
					N->GetVector(Nval, igrid);
					Pval = positionTrans * Pval;
					Nval = normalTrans * Nval;
					V3f Pval2(Pval.x(), Pval.y(), Pval.z());
					V3f Nval2(Nval.x(), Nval.y(), Nval.z());


					// Extract radius, non-interpolation case.
					CqVector3D e1 = diffU<CqVector3D>(P, igrid);
					CqVector3D e2 = diffV<CqVector3D>(P, igrid);
					// Distances from current vertex to diagonal neighbours.
					float d1 = (e1 + e2).Magnitude2();
					float d2 = (e1 - e2).Magnitude2();
					// Choose distance to furtherest diagonal neighbour so
					// that the disks just overlap to produce a surface
					// without holes.  The factor of 0.5 gives the radius
					// rather than diameter.
					float radiusVal = 0.5f*std::sqrt(std::max(d1, d2));


					//rasterise the microbuffer in the integrator
					integrator.clear();
					microRasterize(integrator, Pval2, Nval2, coneAngle,
							maxSolidAngle, *diffusePtc);

					// Make non diffuse surphel in the point cloud
					nonDiffusePtc->addNonDiffuseSurpheltoFile(Pval2,Nval2,radiusVal,phong,integrator.microBuf().getRawPixelData());

					// return the diffuse color bleeding.
					float occ = 0;
					C3f col = integrator.radiosity(Nval2, coneAngle, &occ);
					result->SetColor(CqColor(col.x,col.y,col.z), igrid);

				} // endif varying
			} // endfor shadingpoints
		}
	} else {
		// Couldn't find point cloud, set result to zero.
		// Making no non diffuse surphels

		TqUint igrid = 0;
		do {
			if (!varying || RS.Value(igrid)) {
				result->SetColor(CqColor(0,0,0), igrid);
			}
		} while ((++igrid < shadingPointCount()) && varying);

	}

}

void flushBake3dBrdfCache() {
	// TODO
}

} // namespace Aqsis
