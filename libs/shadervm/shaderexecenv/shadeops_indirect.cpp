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
#include	"../../pointrender/diffuse/DiffusePointOctreeCache.h"
#include	"../../pointrender/nondiffuse/NonDiffusePointOctreeCache.h"
#include	"../../pointrender/nondiffuse/NonDiffusePointOctree.h"
#include	"../../pointrender/nondiffuse/NonDiffusePoint.h"

#include	"../../pointrender/RadiosityIntegrator.h"
#include	"../../pointrender/microbuf_proj_func.h"
#include	"../../pointrender/nondiffuse/approxhemi/hemi_approx_print.h"
#include	"../../pointrender/nondiffuse/approxhemi/PhongModelApprox.h"


#include	"shaderexecenv.h"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

// Cache for previously loaded point clouds
static DiffusePointOctreeCache g_diffusePtcCache;
static NonDiffusePointOctreeCache g_nonDiffusePtcCache;

static int microBufIndex = 0;

/**
 * Helper function of "SO_indirect", calculating the radiance from the nondiffuse pointcloud.
 */

void projectNonDiffusePointCloud(RadiosityIntegrator& integrator,
		NonDiffusePointOctree* nonDiffusePtc, float coneAngle,
		float maxSolidAngle, V3f Pval, V3f Nval, V3f Ival, float scaleIndirectNonDiff) {

	const NonDiffusePointArray& points = nonDiffusePtc->getPointArray();

	float cosConeAngle = cos(coneAngle);
	float sinConeAngle = sin(coneAngle);

	for (int i = 0; i < points.size(); i++) {

		NonDiffusePoint point = points.data[i];

		V3f pointN = point.getNormal();
		V3f pointP = point.getPosition();

		V3f p = pointP-Pval;
		float length = p.length();
		V3f dir = p/length;

		float distRatio = length/point.getRadius();
		if (isnan(distRatio)) {
			Aqsis::log() << warning << "l: " << length << " r: " << point.getRadius() << std::endl;
		}

		if (dot(dir, pointN) < 0) {
			C3f c = point.getHemi()->getRadiosityInDir(-dir,distRatio);
//			C3f c = point.getHemi()->getRadiosityInDir(-dir);
			c = c*(2*M_PI)*scaleIndirectNonDiff;
			float r = point.getRadius();

			integrator.setPointData(reinterpret_cast<float*> (&c));
			renderDisk(integrator, Nval, p, pointN, r, cosConeAngle,
					sinConeAngle);

		}

	}
}

/**
 * Helper function of "SO_indirect", calculating the radiance from the diffuse pointcloud.
 */
void projectDiffusePointCloud(RadiosityIntegrator& integrator,
		const DiffusePointOctree* diffusePtc, float coneAngle,
		float maxSolidAngle, V3f Pval2, V3f Nval2, V3f Ival2) {

	microRasterize(integrator, Pval2, Nval2, coneAngle, maxSolidAngle,
			*diffusePtc);
}

/**
 * The actual ShadeOp "SO_indirect"
 */
void CqShaderExecEnv::SO_indirect(IqShaderData* ptcDiffuse,
		IqShaderData* ptcNonDiffuse, IqShaderData* P, IqShaderData* N,
		IqShaderData* I, IqShaderData* result, IqShader* pShader, int cParams,
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
	DiffusePointOctree* diffusePtc = 0;
	// The pointer to the cloud containging the nondiffuse surphels.
	NonDiffusePointOctree* nonDiffusePtc = 0;
	// Resolution of the microbuffer face.
	int faceRes = 20;
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
	// scaling factor for the indirect nondiffuse light
	float scaleIndirectNonDiff = 1;
	// fileName Diffuse pointcloud
	CqString fileNameDiffusePtc;
	ptcDiffuse->GetString(fileNameDiffusePtc);
	//fileName NonDiffuse pointcloud
	CqString fileNameNonDiffusePtc;
	ptcNonDiffuse->GetString(fileNameNonDiffusePtc);

	/*
	 * Parsing these parameters ...
	 */
	CqString paramName;
	for (int i = 0; i < cParams; i += 2) {
		if (apParams[i]->Type() == type_string) {
			apParams[i]->GetString(paramName, 0);
			IqShaderData* paramValue = apParams[i + 1];
			if (paramName == "coneangle") {
				if (paramValue->Type() == type_float)
					paramValue->GetFloat(coneAngle);
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
			} else if (paramName == "scaleNonDiff") {
				if (paramValue->Type() == type_float) {
					paramValue->GetFloat(scaleIndirectNonDiff);
				}
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
					} else
						P->GetVector(Pval, igrid);

					// Jitter locale coordinate system to avoid banding noise.
					CqVector3D e1 = diffU<CqVector3D> (P, igrid);
					CqVector3D e2 = diffV<CqVector3D> (P, igrid);
					float r1 = m_random.RandomFloat() - 0.5;
					float r2 = m_random.RandomFloat() - 0.5;
					Pval = r1*e1 + r2*e2 + Pval;

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
					 * Calculate the incident color from the point clouds.
					 */

					C3f diffuseCol(0, 0, 0);
					C3f nonDiffuseCol(0, 0, 0);
					integrator.clear();
					if (nonDiffusePtc) {
						projectNonDiffusePointCloud(integrator, nonDiffusePtc,
								coneAngle, maxSolidAngle, Pval2, Nval2, Ival2, scaleIndirectNonDiff);
					}
					if (diffusePtc) {
						projectDiffusePointCloud(integrator, diffusePtc,
								coneAngle, maxSolidAngle, Pval2, Nval2, Ival2);
					}

					C3f col;
					float occ;
					if (phong > 0) {
						col= integrator.realPhongRadiosity(Nval2, Ival2, phong);
					} else {
						col = integrator.realRadiosity(Nval2);
					}

//					col = col * 0.05;
//					Aqsis::log() << warning << "Col is " << col.x << ", "<< col.y <<", "<< col.z << std::endl;

//					const NonDiffusePointArray& points = nonDiffusePtc->getPointArray();
//					NonDiffusePoint p;
//					float min = 9999999999999;
//					for (int i=0; i < points.data.size(); i++) {
//
//						std::stringstream sstr;
//						sstr << "micros/point" << i;
//						writeHemiApproxImage(sstr.str(), 50, points.data[i].getHemi());
//
//						NonDiffusePoint point = points.data[i];
//						V3f diff = point.getPosition()-Pval2;
//						if (diff.length() < min) {
//							min = diff.length();
//							p = point;
//						}
//					}
//					col = p.getHemi()->getRadiosityInDir(-Ival2);

					CqColor res = CqColor(col.x, col.y, col.z);
					result->SetColor(res, igrid);

				} // endif varying
			} // endfor shadingpoints
		}
	} else {
		// Couldn't find point cloud, set result to zero.
		// Making no non diffuse surphels

		TqUint igrid = 0;
		do {
			if (!varying || RS.Value(igrid)) {
				result->SetColor(CqColor(0, 0, 0), igrid);
			}
		} while ((++igrid < shadingPointCount()) && varying);

	}
}

}
