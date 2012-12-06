

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
#include	"../../pointrender/nondiffuse/NonDiffusePointCloudCache.hpp"
#include	"shaderexecenv.h"


namespace Aqsis {

using Imath::V3f;

// Cache for previously loaded point clouds
static PointOctreeCache g_diffusePtcCache;
static NonDiffusePointCloudCache g_nonDiffusePtcCache;

/**
 * Helper function of "SO_indirect", calculating the radiance from the nondiffuse pointcloud.
 */

C3f calcFromNonDiffusePointCloud(	RadiosityIntegrator& integrator,
										NonDiffusePointCloud* nonDiffusePtc,
										float coneAngle,
										float maxSolidAngle,
										int phong,
										V3f Pval2,
										V3f Nval2,
										V3f Ival2) {


	// Extract and calculate the necessary variables.
	int nSurphels = nonDiffusePtc->getNSurphels();
    float cosConeAngle = cos(coneAngle);
    float sinConeAngle = sin(coneAngle);

	// Render all the NonDiffuseSurphels on the microbuffer.
	for (int i=0; i <nSurphels; i++) {

		NonDiffuseSurphel* surphel = nonDiffusePtc->getNonDiffuseSurphel(i);
		V3f surphNormal = *surphel->getNormalPointer();

		V3f p = *surphel->getPositionPointer() - Pval2;
		p = p.normalize();
		C3f c = surphel->getRadiosity(p);
		float r = *surphel->getRadiusPointer();

		integrator.setPointData(reinterpret_cast<const float*>(&c));
		renderDisk(integrator, Nval2, p, surphNormal, r, cosConeAngle, sinConeAngle);
	}


	// Calculate the color from the microbuffer.
	float occ = 0;
	C3f col;
	if (phong > 0) {
		col = integrator.phongRadiosity(Nval2, Ival2, phong, &occ);
	} else {
		col = integrator.radiosity(Nval2,coneAngle, &occ);
	}

	return col;
}




/**
 * Helper function of "SO_indirect", calculating the radiance from the diffuse pointcloud.
 */
C3f calcFromDiffusePointCloud(	RadiosityIntegrator& integrator,
									const PointOctree* diffusePtc,
									float coneAngle,
									float maxSolidAngle,
									int phong,
									V3f Pval2,
									V3f Nval2,
									V3f Ival2) {
	microRasterize(integrator, Pval2, Nval2, coneAngle,	maxSolidAngle, *diffusePtc);

	float occ = 0;
	C3f col;
	if (phong > 0) {
		col = integrator.phongRadiosity(Nval2, Ival2, phong, &occ);
	} else {
		col = integrator.radiosity(Nval2,coneAngle, &occ);
	}

	return col;
}



/**
 * The actual ShadeOp "SO_indirect"
 */
void CqShaderExecEnv::SO_indirect(IqShaderData* P,
									IqShaderData* N,
									IqShaderData* I,
									IqShaderData* samples,
									IqShaderData* result,
									IqShader* pShader,
									int cParams,
									IqShaderData** apParams)
{
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
				float exponent = 10;
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
		nonDiffusePtc = g_nonDiffusePtcCache.find(fileNameNonDiffusePtc);
	}


	// TODO: Debug statement
//	Aqsis::log() << warning << "Phong exponent: " << phong << " Faceres: "<< faceRes <<+ "\n";


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

	if (diffusePtc || nonDiffusePtc) {

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
					CqVector3D Ival;
					N->GetVector(Nval, igrid);
					I->GetVector(Ival, igrid);
					Pval = positionTrans * Pval;
					Nval = normalTrans * Nval;
					V3f Pval2(Pval.x(), Pval.y(), Pval.z());
					V3f Nval2(Nval.x(), Nval.y(), Nval.z());
					V3f Ival2(Ival.x(), Ival.y(), Ival.z());

					/**
					 * Calculate the incident color from the diffuse point cloud.
					 */


					C3f diffuseCol(0,0,0);
					C3f nonDiffuseCol(0,0,0);
					integrator.clear();
					if (diffusePtc) {
						diffuseCol = calcFromDiffusePointCloud(integrator,
								diffusePtc, coneAngle, maxSolidAngle, phong, Pval2, Nval2, Ival2);

					}
					if (nonDiffusePtc) {
						nonDiffuseCol = calcFromNonDiffusePointCloud(integrator,
								nonDiffusePtc, coneAngle, maxSolidAngle, phong, Pval2, Nval2, Ival2);

						Aqsis::log() << warning << "Done nondiffuse shadingpoint "<<igrid <<"/"<< npoints
								<<" (" << nonDiffuseCol.x << " " << nonDiffuseCol.y <<" " << nonDiffuseCol.z << ")"<< std::endl;
					}


					CqColor col = CqColor(	diffuseCol.x+nonDiffuseCol.x,
											diffuseCol.y+nonDiffuseCol.y,
											diffuseCol.z+nonDiffuseCol.z);
					result->SetColor(col, igrid);



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

}
