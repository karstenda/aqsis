/*
 * NonDiffusePointOctree.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#include <cassert>
#include <cmath>
#include <cstring>

#include <Partio.h>

#include <aqsis/util/logging.h>

#include "NonDiffusePointOctree.h"
#include "NonDiffusePoint.h"
#include "approxhemi/HemiApprox.h"
#include "approxhemi/CubeMapApprox.h"
#include "approxhemi/SpherHarmonApprox.h"
#include "approxhemi/PhongModelApprox.h"

namespace Aqsis {

using std::vector;
using Imath::V3f;
using Imath::C3f;
using Imath::Box3f;


static void releasePartioFile(Partio::ParticlesInfo* file) {
	if (file)
		file->release();
}

bool loadNonDiffusePointFile(NonDiffusePointArray& points, const std::string& fileName) {
	namespace Pio = Partio;
	boost::shared_ptr < Pio::ParticlesData > ptFile(Pio::read(fileName.c_str()), releasePartioFile);
	if (!ptFile)
		return false;
	// Look for the necessary attributes in the file
	Pio::ParticleAttribute posAttr;
	Pio::ParticleAttribute norAttr;
	Pio::ParticleAttribute areaAttr;
	Pio::ParticleAttribute hemiAttr;
	if (!ptFile->attributeInfo("position", posAttr) || !ptFile->attributeInfo(
			"normal", norAttr) || !ptFile->attributeInfo("_area", areaAttr)
			|| !ptFile->attributeInfo("_hemi", hemiAttr)) {
		Aqsis::log() << "Couldn't find required attribute in \"" << fileName
				<< "\"\n";
		return false;
	}
	// Check types
	if (posAttr.type != Pio::VECTOR || norAttr.type != Pio::VECTOR
			|| areaAttr.type != Pio::FLOAT || areaAttr.count != 1
			|| hemiAttr.type != Pio::FLOAT) {
		Aqsis::log() << error << "Point attribute count or type wrong in \"" << fileName
				<< "\"\n";
		return false;
	}

	int npts = ptFile->numParticles();
	std::vector<NonDiffusePoint>& data = points.data;
	data.resize(data.size() + npts);

	// Iterate over all particles
	Pio::ParticleAccessor posAcc(posAttr);
	Pio::ParticleAccessor norAcc(norAttr);
	Pio::ParticleAccessor areaAcc(areaAttr);
	Pio::ParticleAccessor hemiAcc(hemiAttr);
	Pio::ParticlesData::const_iterator pt = ptFile->begin();

	pt.addAccessor(posAcc);
	pt.addAccessor(norAcc);
	pt.addAccessor(areaAcc);
	pt.addAccessor(hemiAcc);


	for (int i=0; pt != ptFile->end(); ++pt, i++) {
		const float* P = reinterpret_cast<const float*>(posAcc.basePointer+pt.index*posAcc.stride);
		const float* N = reinterpret_cast<const float*>(norAcc.basePointer+pt.index*norAcc.stride);
		const float* A = reinterpret_cast<const float*>(areaAcc.basePointer+pt.index*areaAcc.stride);
		const float* H = reinterpret_cast<const float*>(hemiAcc.basePointer+pt.index*hemiAcc.stride);


		V3f position(P[0],P[1],P[2]);
		V3f normal(N[0],N[1],N[2]);
		float radius = sqrtf(A[0] / M_PI);

		HemiApprox::Type type = (HemiApprox::Type) H[0];
		HemiApprox* hemiApprox;
		switch (type) {
		case HemiApprox::CubeMap:
			hemiApprox = new CubeMapApprox(H,hemiAttr.count);
			break;
		case HemiApprox::SpherHarmon:
			hemiApprox = new SpherHarmonApprox(H,hemiAttr.count);
			break;
		case HemiApprox::PhongModel:
			hemiApprox = new PhongModelApprox(H,hemiAttr.count);
			break;
		default:
			Aqsis::log() << warning << "No implementation for approximation type: "
				<< type << std::endl;
			break;
		}

		NonDiffusePoint ndPoint(position, normal, radius, hemiApprox);
		data[i] = ndPoint;
	}
	return true;
}


NonDiffusePointOctree::NonDiffusePointOctree(const NonDiffusePointArray& points) :
	m_root(0), points(points) {

	size_t npoints = points.size();
	// Super naive, recursive top-down construction.
	//
	// TODO: Investigate bottom-up construction based on sorting in
	// order of space filling curve.
	Box3f bound;
	std::vector<const NonDiffusePoint*> workspace(npoints);
	for (size_t i = 0; i < npoints; ++i) {
		const NonDiffusePoint* p = &points.data[i];
		bound.extendBy(p->getPosition());
		workspace[i] = &points.data[i];
	}
	// We make octree bound cubic rather than fitting the point cloud
	// tightly.  This improves the distribution of points in the octree
	// nodes and reduces artifacts when groups of points are aggregated
	// in the internal nodes.
	//
	// If we *don't* do this and we have a rectangular (non-cubic)
	// bound, we end up with a lot more points in one direction inside
	// a node than another.  This means the aggregated averaged point -
	// intended to represent the collection - is in the middle, but
	// with lots of room on either side:
	//
	// +-----------+   ----->    +----/^\----+
	// | o o o o o |  aggregate  |   | . |   |
	// +-----------+             +----\_/----+
	//
	//   <------->                   <--->
	// even distribution           all in middle :(
	//
	// That is, there will be large gaps between neighbouring disks,
	// which gives large transparent gaps in the microrendered surface.
	// Obviously a bad thing!
	V3f d = bound.size();
	V3f c = bound.center();
	float maxDim2 = std::max(std::max(d.x, d.y), d.z) / 2;
	bound.min = c - V3f(maxDim2);
	bound.max = c + V3f(maxDim2);

//  TODO @karstenda For now, just stick to the pointarray.
	m_root = makeTree(0, workspace, bound);
}


NonDiffusePointOctree::Node* NonDiffusePointOctree::makeTree(int depth,
		const std::vector<const NonDiffusePoint*>& points, const Box3f& bound) {


	size_t npoints = points.size();
	assert(npoints != 0);
	Node* node = new Node;
	node->bound = bound;
	V3f c = bound.center();
	node->center = c;
	V3f diag = bound.size();
	node->boundRadius = diag.length() / 2.0f;
	node->npoints = 0;
	size_t pointsPerLeaf = 8;


	// Limit max depth of tree to prevent infinite recursion when
	// greater than pointsPerLeaf points lie at the same position in
	// space.  floats effectively have 24 bit of precision in the
	// significand, so there's never any point splitting more than 24
	// times.

	int maxDepth = 24;
	if (npoints <= pointsPerLeaf || depth >= maxDepth) {
		// Small number of child points: make this a leaf node and
		// store the points directly in the data member.
		node->npoints = npoints;
		// Copy over data into node.
		node->data.reset(new const NonDiffusePoint*[npoints]);
		float sumA = 0;
		V3f sumP(0);
		V3f sumN(0);
		// TODO Could be smarter
		HemiApprox* aggHemi = points[0]->getHemi()->getDarkEquivalent();
		for (size_t j = 0; j < npoints; ++j) {
			node->data[j] = points[j];
			// compute averages (area weighted)
			float A = points[j]->getRadius() * points[j]->getRadius();
			sumA += A;
			sumP += A * points[j]->getPosition();
			sumN += A * points[j]->getNormal();
			// TODO take surface into account.
			aggHemi->add(points[j]->getHemi());
//			sumCol += A * C3f(0, 0, 0);
		}
		node->aggP = 1.0f / sumA * sumP;
		node->aggN = sumN.normalized();
		node->aggR = sqrtf(sumA);
		node->aggHemi = aggHemi;
//		node->aggCol = 1.0f / sumA * sumCol;
		return node;
	}
	// allocate extra workspace for storing child points (ugh!)
	vector< vector<const NonDiffusePoint*> >workspace(8);

	// Partition points into the eight child nodes
	for (size_t i = 0; i < npoints; ++i) {
		float px = points[i]->getPosition().x;
		float py = points[i]->getPosition().y;
		float pz = points[i]->getPosition().z;
		int cellIndex = 4 * (pz > c.z) + 2 * (py > c.y) + (px > c.x);
		workspace[cellIndex].push_back(points[i]);
	}

	// Recursively generate child nodes and compute position, normal
	// and radius for the current node.
	float sumA = 0;
	V3f sumP(0);
	V3f sumN(0);
	C3f sumCol(0);
	// TODO Could be smarter
	HemiApprox* sumHemi = points[0]->getHemi()->getDarkEquivalent();
	for (int i = 0; i < 8; ++i) {
		if (workspace[i].size() == 0)
			continue;

		Box3f bnd;
		bnd.min.x = (i % 2 == 0) ? bound.min.x : c.x;
		bnd.min.y = ((i / 2) % 2 == 0) ? bound.min.y : c.y;
		bnd.min.z = ((i / 4) % 2 == 0) ? bound.min.z : c.z;
		bnd.max.x = (i % 2 == 0) ? c.x : bound.max.x;
		bnd.max.y = ((i / 2) % 2 == 0) ? c.y : bound.max.y;
		bnd.max.z = ((i / 4) % 2 == 0) ? c.z : bound.max.z;
		Node* child = makeTree(depth + 1, workspace[i], bnd);
		node->children[i] = child;
		// Weighted average with weight = disk surface area.
		float A = child->aggR * child->aggR;
		sumA += A;
		sumP += A * child->aggP;
		sumN += A * child->aggN;
		// TODO take area into account
		sumHemi->add(child->aggHemi);
//		sumCol += A * child->aggCol;
	}
	node->aggP = 1.0f / sumA * sumP;
	node->aggN = sumN.normalized();
	node->aggR = sqrtf(sumA);
	// TODO take area into account
	node->aggHemi = sumHemi;
//	node->aggCol = 1.0f / sumA * sumCol;
	return node;
}


void NonDiffusePointOctree::deleteTree(Node* n) {
	if (!n)
		return;
	for (int i = 0; i < 8; ++i)
		deleteTree(n->children[i]);
	delete n;
}

NonDiffusePointOctree::~NonDiffusePointOctree() {
	deleteTree( m_root);
}

}
