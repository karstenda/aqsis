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


#include	"../microbuffer.h"
#include	"NonDiffusePointCloud.hpp"


using namespace std;

namespace Aqsis {

/**
 * This constructor is used to create a new NonDiffusePointCloud. If the file alread exists,
 * it will be overwritten.
 */
NonDiffusePointCloud::NonDiffusePointCloud(string filename,int faceRes, int nchans):
		filename(filename),
		microBufferSize(MicroBuf::Face_end*nchans*faceRes*faceRes*sizeof(float)),
		surphelSize(microBufferSize+2*sizeof(V3f)+sizeof(int)+sizeof(float)),
		currentSurphelIndex(0),
		currentSurphel(faceRes),
		nSurphels(0),
		filehandle(filename.c_str(),  ios::out  | ios::in | ios::binary | ios::trunc) {

	if (filehandle.is_open()){
		filehandle.write((char*) &faceRes, sizeof(int));
		filehandle.write((char*) &nchans, sizeof(int));
	} else {
		Aqsis::log() << error << "Unable to open " << filename << std::endl;
		exit(1);
	}
	Aqsis::log() << warning << "Created " << filename << std::endl;

}

/**
 * This constructor is used to load an existing NonDiffusePointCloud from a file.
 */
NonDiffusePointCloud::NonDiffusePointCloud(string filename, int faceRes, int nchans, long surphelDataSize):
		filename(filename),
		microBufferSize(MicroBuf::Face_end*nchans*faceRes*faceRes*sizeof(float)),
		surphelSize(microBufferSize+2*sizeof(V3f)+sizeof(int)+sizeof(float)),
		currentSurphelIndex(0),
		currentSurphel(faceRes),
		nSurphels(surphelDataSize/surphelSize),
		filehandle(filename.c_str(),  ios::out  | ios::in | ios::binary | ios::ate ) {

	Aqsis::log() << warning << "Opened " << filename << " containing " << nSurphels << " surphels." << std::endl;
	if (!filehandle.is_open()) {
		Aqsis::log() << error << "Unable to open " << filename << std::endl;
		exit(1);
	}
}



void NonDiffusePointCloud::addSurphel(V3f position, V3f normal, float radius, int phong, float* pixels) {

	if (filehandle.is_open()){
		filehandle.seekp(0,ios::end);
		filehandle.write ((char*) &position, sizeof(V3f));
		filehandle.write ((char*) &normal, sizeof(V3f));
		filehandle.write ((char*) &radius, sizeof(float));
		filehandle.write ((char*) &phong, sizeof(int));
		filehandle.write ((char*) pixels, microBufferSize);
		nSurphels++;
	} else {
		Aqsis::log() << error << "Unable to write NonDiffuseSurphel to " << filename << std::endl;
		exit(1);
	}
}

int NonDiffusePointCloud::getNSurphel() {
	return nSurphels;
}

int NonDiffusePointCloud::getCurrentSurphelIndex() {
	return currentSurphelIndex;
}


NonDiffuseSurphel* NonDiffusePointCloud::getSurphel(int surphelIndex) {
	if (surphelIndex < nSurphels) {
		filehandle.seekg(2*sizeof(int)+surphelIndex*surphelSize);
		readCurrentSurphelFromFile();
		return &currentSurphel;
	} else {
		Aqsis::log() << warning << "No such NonDiffuseSurphel in cloud from  " << filename << std::endl;
		exit(1);
	}
}

void NonDiffusePointCloud::setCurrentSurphelIndex(int surphelIndex) {

	if (surphelIndex < nSurphels) {
		currentSurphelIndex = surphelIndex;
	}
}

NonDiffuseSurphel* NonDiffusePointCloud::getNextSurphel() {
	if (currentSurphelIndex < nSurphels) {
		readCurrentSurphelFromFile();
		currentSurphelIndex++;
		return &currentSurphel;
	} else {
		Aqsis::log() << error << "No more NonDiffuseSurphels in cloud from  " << filename << std::endl;
		exit(1);
	}
}

void NonDiffusePointCloud::readCurrentSurphelFromFile() {

	if (filehandle.is_open()){
		filehandle.seekg(2*sizeof(int)+currentSurphelIndex*surphelSize);
		filehandle.read((char*) currentSurphel.getPositionPointer(), sizeof(V3f));
		filehandle.read((char*) currentSurphel.getNormalPointer(), sizeof(V3f));
		filehandle.read((char*) currentSurphel.getRadiusPointer(), sizeof(float));
		filehandle.read((char*) currentSurphel.getPhongPointer(), sizeof(int));
		filehandle.read((char*) currentSurphel.getPixelPointer(), microBufferSize);
	} else {
		  Aqsis::log() << error << "Unable to read NonDiffuseSurphel from " << filename << std::endl;
		  exit(1);
	}
}

NonDiffusePointCloud::~NonDiffusePointCloud() {
	filehandle.close();
}

}
