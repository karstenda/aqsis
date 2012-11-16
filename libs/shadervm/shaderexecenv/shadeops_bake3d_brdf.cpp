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

#include	"../../pointrender/microbuffer.h"
#include	"../../pointrender/nondiffuse/NonDiffusePointCloud.hpp"
#include	"shaderexecenv.h"

namespace Aqsis {

using Imath::V3f;

// Cache for previously loaded point clouds
static PointOctreeCache g_pointOctreeCache;

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

void CqShaderExecEnv::SO_bake3d_brdf(	IqShaderData* ptc,
											IqShaderData* channels,
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
	const PointOctree* pointTree = 0;
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
	int phong = 0;

	/*
	 * Loading the octree of diffuse surphels.
	 */
	CqString fileName;
	ptc->GetString(fileName,0);
	pointTree = g_pointOctreeCache.find(fileName);

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
		} else if (paramName == "filename") {
			if (paramValue->Type() == type_string) {
				CqString fileName;
				paramValue->GetString(fileName, 0);
				pointTree = g_pointOctreeCache.find(fileName);
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
				float exponent = 10;
				paramValue->GetFloat(exponent);
				phong = std::max(0, static_cast<int> (exponent));
			}
		}
	}

	// TODO: Debug statement
//	Aqsis::log() << warning << "Phong exponent: " << phong << "\n";


	// Non diffuse Point cloud
	NonDiffusePointCloud nonDiffusePointCloud("NonDiffuseCloud.bin", faceRes, 5);

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

	if (pointTree) {

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

					// Microgrids sometimes meet each other at an acute angle.
					// Computing occlusion at the vertices where the grids meet is
					// then rather difficult because an occluding disk passes
					// exactly through the point to be occluded.  This usually
					// results in obvious light leakage from the other side of the
					// surface.
					//
					// To avoid this problem, we modify the position of any
					// vertices at the edges of grids by moving them inward
					// slightly.
					//
					// TODO: Make adjustable?

					const float edgeShrink = 0.2f;
					if (u == 0)
						uinterp = edgeShrink;
					else if (u == m_uGridRes) {
						uinterp = 1 - edgeShrink;
						--u;
					}
					if (v == 0)
						vinterp = edgeShrink;
					else if (v == m_vGridRes) {
						vinterp = 1 - edgeShrink;
						--v;
					}
					if (uinterp != 0 || vinterp != 0) {
						CqVector3D _P1;
						CqVector3D _P2;
						CqVector3D _P3;
						CqVector3D _P4;
						int uSize = m_uGridRes + 1;
						P->GetPoint(_P1, v * uSize + u);
						P->GetPoint(_P2, v * uSize + u + 1);
						P->GetPoint(_P3, (v + 1) * uSize + u);
						P->GetPoint(_P4, (v + 1) * uSize + u + 1);
						Pval = (1 - vinterp) * (1 - uinterp) * _P1 + (1
								- vinterp) * uinterp * _P2 + vinterp * (1
								- uinterp) * _P3 + vinterp * uinterp * _P4;
					} else {
						P->GetVector(Pval, igrid);
					}

					// Calculate the position and the normal of the shadingpoint
					CqVector3D Nval;
					N->GetVector(Nval, igrid);
					Pval = positionTrans * Pval;
					Nval = normalTrans * Nval;
					V3f Pval2(Pval.x(), Pval.y(), Pval.z());
					V3f Nval2(Nval.x(), Nval.y(), Nval.z());

					//rasterise the microbuffer in the integrator
					integrator.clear();
					microRasterize(integrator, Pval2, Nval2, coneAngle,
							maxSolidAngle, *pointTree);

					// Make non diffuse surphel in the point cloud
					nonDiffusePointCloud.addPoint(Pval2,Nval2,integrator.microBuf().getRawPixelData());

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
