/*
 * NonDiffusePointCloudCache.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: karsten
 */

#include <aqsis/aqsis.h>
#include <aqsis/util/logging.h>
#include <fstream>

#include "../microbuffer.h"
#include "NonDiffusePointCloudCache.hpp"

using namespace std;

namespace Aqsis {

NonDiffusePointCloud* NonDiffusePointCloudCache::findOrCreate(const std::string& fileName, int faceRes, int nchans) {

	MapType::const_iterator i = m_cache.find(fileName);
	if (i == m_cache.end()) {

		// Try to open the file
		// TODO: Path handling
		boost::shared_ptr <NonDiffusePointCloud> cloud;
		cloud.reset(new NonDiffusePointCloud(fileName, faceRes, nchans));
		// Insert into map.  If we couldn't load the file, we insert
		// a null pointer to record the failure.
		m_cache.insert(MapType::value_type(fileName, cloud));
		return cloud.get();
	}
	return i->second.get();
}

NonDiffusePointCloud* NonDiffusePointCloudCache::find(const std::string& fileName) {

	MapType::const_iterator i = m_cache.find(fileName);
	if (i == m_cache.end()) {
		// Try to open the file
		// TODO: Path handling
		boost::shared_ptr <NonDiffusePointCloud> cloud;
		fstream filehandle(fileName.c_str(),  ios::out  | ios::in | ios::binary | ios::ate );

		// Load the setting of the point cloud.
		int faceRes;
		int nchans;
		if (filehandle.is_open()) {
			filehandle.seekg(0, ios::beg);
			filehandle.read((char*) &faceRes, sizeof(int));
			filehandle.read((char*) &nchans, sizeof(int));
			filehandle.seekp(0,ios::end);

			long surphelDataSize = filehandle.tellp() - 2*sizeof(int);
			cloud.reset(new NonDiffusePointCloud(fileName, faceRes, nchans, surphelDataSize));
			filehandle.close();
		} else {
			Aqsis::log() << warning << "Unable to open " << fileName << std::endl;
		}

		// Insert into map.  If we couldn't load the file, we insert
		// a null pointer to record the failure.
		m_cache.insert(MapType::value_type(fileName, cloud));
		return cloud.get();
	}
	return i->second.get();
}

void NonDiffusePointCloudCache::clear() {
	m_cache.clear();
}

}
