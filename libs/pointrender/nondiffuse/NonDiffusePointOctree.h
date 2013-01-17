/*
 * NonDiffusePointOctree.h
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINTOCTREE_H_
#define NONDIFFUSEPOINTOCTREE_H_


#include <vector>

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>

#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>

#include "PointArray.h"

namespace Aqsis {

/// Load in point array from aqsis point cloud file format
///
/// The point cloud file must at a minimum include a float attribute "_area".
/// The position, normal, area and optionally radiosity ("_radiosity") will be
/// loaded into the PointArray which is returned.  The points will be
/// _appended_ to the provided points PointArray.  The return value is true on
/// success, false on error.
bool loadNonDiffusePointFile(PointArray& points, const std::string& fileName);

class NonDiffusePointOctree {

public:
	/// Tree node
	///
	/// Leaf nodes have npoints > 0, specifying the number of child points
	/// contained.
	struct Node {
		Node() :
			bound(), center(0), boundRadius(0), aggP(0), aggN(0), aggR(0),
					aggCol(0), npoints(0), data() {
			children[0] = children[1] = children[2] = children[3] = 0;
			children[4] = children[5] = children[6] = children[7] = 0;
		}

		/// Data derived from octree bounding box
		Imath::Box3f bound;
		Imath::V3f center;
		float boundRadius;
		// Crude aggregate values for position, normal and radius
		Imath::V3f aggP;
		Imath::V3f aggN;
		float aggR;
		Imath::C3f aggCol;
		// Child nodes, to be indexed as children[z][y][x]
		Node* children[8];
		// bool used;
		/// Number of child points for the leaf node case
		int npoints;
		// Collection of points in leaf.
		boost::scoped_array<float> data;
	};


private:

	Node* m_root;
	int m_dataSize;
	PointArray points;

public:

	/// Construct tree from array of points.
	NonDiffusePointOctree(const PointArray& points);

	~NonDiffusePointOctree();

	// @karstenda
	NonDiffusePointOctree(): m_root(0), m_dataSize(), points() {	}
	PointArray& getPointArray() {
		return points;
	}

	/// Get root node of tree
	const Node* root() const {
		return m_root;
	}

	/// Get number of floats representing each point.
	int dataSize() const {
		return m_dataSize;
	}

private:
	/// Build a tree node from the given points
	///
	/// \param depth - depth of the node to be created
	/// \param points - pointers to point data
	/// \param npoints - number of points in points array
	/// \param dataSize - number of floats representing each point
	static Node* makeTree(int depth, const float** points, size_t npoints,
			int dataSize, const Imath::Box3f& bound);

	/// Recursively delete tree, depth first.
	static void deleteTree(Node* n);

};

}
#endif /* NONDIFFUSEPOINTOCTREE_H_ */
