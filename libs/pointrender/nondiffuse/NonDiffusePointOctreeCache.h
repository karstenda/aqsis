/*
 * NonDiffusePointOctreeCache.h
 *
 *  Created on: Jan 10, 2013
 *      Author: Karsten Daemen
 */

#ifndef NONDIFFUSEPOINTOCTREECACHE_H_
#define NONDIFFUSEPOINTOCTREECACHE_H_

#include <map>

#include <boost/shared_ptr.hpp>
#include "NonDiffusePointOctree.h"

namespace Aqsis {

class NonDiffusePointOctreeCache {

private:

	typedef std::map<std::string, boost::shared_ptr<NonDiffusePointOctree> > MapType;
	MapType m_cache;

public:
	/// Find a cached point octree by file name
	///
	/// Returns a null pointer if the file couldn't be found or opened.
	///
	/// TODO: Search path handling.
	NonDiffusePointOctree* find(const std::string& fileName);

	/// Clear all trees from the cache
	void clear();

};

}

#endif /* NONDIFFUSEPOINTOCTREECACHE_H_ */
