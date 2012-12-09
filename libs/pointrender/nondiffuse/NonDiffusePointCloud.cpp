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


/**
 * This constructor is used to create a new NonDiffusePointCloud. If the file alread exists,
 * it will be overwritten.
 */
NonDiffusePointCloud::NonDiffusePointCloud(std::string filename,int faceRes, int nchans):
		surphels(0),
		fileHandler(filename, faceRes, nchans),
		nSurphels(0),
		cacheStart(0) {

	nSurphels = fileHandler.getNSurphels();
	surphels = new vector<NonDiffuseSurphel>(nSurphels,NonDiffuseSurphel(faceRes));
}


/**
 * This constructor is used to load an existing NonDiffusePointCloud from a file.
 */
NonDiffusePointCloud::NonDiffusePointCloud(std::string filename):
		surphels(0),
		fileHandler(filename),
		nSurphels(0),
		cacheStart(0){

	nSurphels = fileHandler.getNSurphels();
	int faceRes = fileHandler.getFaceRes();
	surphels = new vector<NonDiffuseSurphel>(nSurphels,NonDiffuseSurphel(faceRes));
	fileHandler.loadNonDiffuseSurphels(0,nSurphels,surphels);
}

int NonDiffusePointCloud::getNSurphels() {
	return surphels->size();
}

void NonDiffusePointCloud::addNonDiffuseSurpheltoFile(V3f position, V3f normal, float radius, int phong, float* pixels) {
	fileHandler.addNonDiffuseSurphel(position,normal,radius,phong,pixels);
}

NonDiffuseSurphel* NonDiffusePointCloud::getNonDiffuseSurphel(int index) {
	return &surphels->at(index);
}

void NonDiffusePointCloud::reloadFromFile() {
	fileHandler.loadNonDiffuseSurphels(0,nSurphels,surphels);
}

NonDiffusePointCloud::~NonDiffusePointCloud() {
	delete surphels;
}

}
