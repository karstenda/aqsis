/*
 * NonDiffusePointCloud.h
 *
 *  Created on: Nov 2, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINTCLOUD_HPP_
#define NONDIFFUSEPOINTCLOUD_HPP_

#include	"../RadiosityIntegrator.h"
#include	"NonDiffuseFileHandler.hpp"
#include	"NonDiffuseSurphel.hpp"


namespace Aqsis {

class NonDiffusePointCloud {

public:
	// CacheSize in bytes.
	static long cacheSize;

private:

	std::vector<NonDiffuseSurphel>* surphels;
	NonDiffuseFileHandler fileHandler;
	long nSurphels;
	int surphelSize;			// in bytes.
	long cacheStart;			// in surphelIndex.
	long cacheCapacity;			// in number of surphels.

public:

	/**
	 * This constructor is used to create a new NonDiffusePointCloud. If the file alread exists,
	 * it will be overwritten.
	 */
	NonDiffusePointCloud(std::string filename,int faceRes, int nchans);

	/**
	 * This constructor is used to load an existing NonDiffusePointCloud from a file.
	 */
	NonDiffusePointCloud(std::string filename);
	virtual ~NonDiffusePointCloud();

	int getNSurphels();
	void addNonDiffuseSurpheltoFile(Imath::V3f position, Imath::V3f normal, float radius, int phong, float* pixels);
	void writeToFile();
	NonDiffuseSurphel* getNonDiffuseSurphel(long index);
	void reloadFromFile();
};

}
#endif /* NONDIFFUSEPOINTCLOUD_H_ */
