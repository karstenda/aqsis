/*
 * NonDiffusePointCloud.cpp
 *
 *  Created on: Nov 2, 2012
 *      Author: Karsten
 */

#include	<string>
#include	<sys/stat.h>
#include	<sys/types.h>

#include	<aqsis/aqsis.h>
#include	<aqsis/util/logging.h>

#include	"NonDiffuseFileHandler.hpp"
#include	"NonDiffusePointCloud.hpp"

using namespace std;

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

long NonDiffusePointCloud::cacheSize = 1024 * 1024 * 1024;

/**
 * This constructor is used to create a new NonDiffusePointCloud. If the file alread exists,
 * it will be overwritten.
 */
NonDiffusePointCloud::NonDiffusePointCloud(std::string filename, int faceRes,
		int nchans) :
	surphels(0), fileHandler(filename, faceRes, nchans), nSurphels(0),
			surphelSize(0), cacheStart(0), cacheCapacity(0) {

	nSurphels = fileHandler.getNSurphels();
	surphelSize = fileHandler.getSurphelSize();

	if (cacheCapacity > nSurphels) {
		surphels = new vector<NonDiffuseSurphel> (nSurphels, NonDiffuseSurphel(
				faceRes));
		fileHandler.loadNonDiffuseSurphels(0, nSurphels, surphels);
		cacheCapacity = nSurphels;
	} else {
		surphels = new vector<NonDiffuseSurphel> (cacheCapacity,
				NonDiffuseSurphel(faceRes));
		fileHandler.loadNonDiffuseSurphels(0, cacheCapacity, surphels);
	}

	Aqsis::log() << warning << "Cache of " << filename << " has size of "
			<< cacheCapacity << " surphels." << std::endl;
}

/**
 * This constructor is used to load an existing NonDiffusePointCloud from a file.
 */
NonDiffusePointCloud::NonDiffusePointCloud(std::string filename) :
	surphels(0), fileHandler(filename), nSurphels(0), surphelSize(0),
			cacheStart(0), cacheCapacity(0) {

	nSurphels = fileHandler.getNSurphels();
	surphelSize = fileHandler.getSurphelSize();

	cacheCapacity = cacheSize / surphelSize;
	int faceRes = fileHandler.getFaceRes();

	if (cacheCapacity > nSurphels) {
		surphels = new vector<NonDiffuseSurphel> (nSurphels, NonDiffuseSurphel(
				faceRes));
		fileHandler.loadNonDiffuseSurphels(0, nSurphels, surphels);
		cacheCapacity = nSurphels;
	} else {
		surphels = new vector<NonDiffuseSurphel> (cacheCapacity,
				NonDiffuseSurphel(faceRes));
		fileHandler.loadNonDiffuseSurphels(0, cacheCapacity, surphels);
	}

	Aqsis::log() << warning << "Cache of " << filename << " has size of "
			<< cacheCapacity << " surphels." << std::endl;
}

int NonDiffusePointCloud::getNSurphels() {
	return nSurphels;
}

void NonDiffusePointCloud::addNonDiffuseSurpheltoFile(V3f position, V3f normal,
		float radius, int phong, float* pixels) {
	fileHandler.addNonDiffuseSurphel(position, normal, radius, phong, pixels);
}

NonDiffuseSurphel* NonDiffusePointCloud::getNonDiffuseSurphel(long index) {

	if ((index - cacheStart) >= cacheCapacity || (index - cacheStart) < 0) {

		cacheStart = index / cacheCapacity;
		cacheStart = cacheCapacity * cacheStart;

		if (cacheStart + cacheCapacity > nSurphels) {
			fileHandler.loadNonDiffuseSurphels(cacheStart, nSurphels
					- cacheStart, surphels);
		} else {
			fileHandler.loadNonDiffuseSurphels(cacheStart, cacheCapacity,
					surphels);
		}
	}

	return &surphels->at(index - cacheStart);
}

void NonDiffusePointCloud::reloadFromFile() {

	if (cacheStart + cacheCapacity > nSurphels) {
		fileHandler.loadNonDiffuseSurphels(cacheStart, nSurphels - cacheStart,
				surphels);
	} else {
		fileHandler.loadNonDiffuseSurphels(cacheStart, cacheCapacity, surphels);
	}
}

void NonDiffusePointCloud::writeToFile() {
	fileHandler.flushBuffer();
}

NonDiffusePointCloud::~NonDiffusePointCloud() {
	delete surphels;
}

}
