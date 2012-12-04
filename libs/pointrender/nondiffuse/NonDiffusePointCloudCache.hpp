/*
 * NonDiffusePointCloudCache.hpp
 *
 *  Created on: Dec 2, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINTCLOUDCACHE_HPP_
#define NONDIFFUSEPOINTCLOUDCACHE_HPP_


#include "NonDiffusePointCloud.hpp"

namespace Aqsis {

class NonDiffusePointCloudCache {

private:

	typedef std::map<std::string, boost::shared_ptr<NonDiffusePointCloud> > MapType;
	MapType m_cache;

public:

	/// Find a cached NonDiffusePointCloud by file name.
	///
	/// Returns a null pointer if the file couldn't be found or opened.
	///
	/// TODO: Search path handling.
	NonDiffusePointCloud* find(const std::string& fileName);
	NonDiffusePointCloud* findOrCreate(const std::string& fileName, int faceRes, int nchans);

	/// Clear all trees from the cache
	void clear();

};

}
#endif /* NONDIFFUSEPOINTCLOUDCACHE_HPP_ */
