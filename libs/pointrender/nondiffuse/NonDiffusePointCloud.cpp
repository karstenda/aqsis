/*
 * NonDiffusePointCloud.cpp
 *
 *  Created on: Nov 2, 2012
 *      Author: karsten
 */

#include	<string>
#include	<sys/stat.h>
#include	<sys/types.h>
#include	<fstream>

#include	<aqsis/aqsis.h>
#include	<aqsis/util/logging.h>


#include	"../microbuffer.h"
#include	"NonDiffusePointCloud.hpp"


using namespace std;

namespace Aqsis {

NonDiffusePointCloud::NonDiffusePointCloud(std::string filename,int faceRes, int nchans):
		filename(filename),
		microBufferSize(MicroBuf::Face_end*nchans*faceRes*faceRes*sizeof(float)),
		currentSurphelIndex(0),
		currentSurphel(faceRes, nchans) {

}


bool NonDiffusePointCloud::isValid() {
	return true;
}




void NonDiffusePointCloud::addSurphel(V3f position, V3f normal, int phong, float* pixels) {
	writeToFile(position, normal, phong,  pixels);
}

void NonDiffusePointCloud::writeToFile(V3f position, V3f normal, int phong, float* pixels) {

	ofstream myFile(filename.c_str(), ios::out | ios::binary | ios::app);
	if (myFile.is_open()){
		myFile.write ((char*) &position, sizeof(V3f));
		myFile.write ((char*) &normal, sizeof(V3f));
		myFile.write ((char*) pixels, microBufferSize);
		myFile.close();
	} else {
		Aqsis::log() << warning << "Unable to write microbuffer to " << filename << std::endl;
	}
}

NonDiffuseSurphel* NonDiffusePointCloud::getCurrentSurphel() {
	return &currentSurphel;
}

void NonDiffusePointCloud::setCurrentSurphelIndex(int surphelIndex) {
	currentSurphelIndex = surphelIndex;
	readSurphelFromFile(surphelIndex*(2*sizeof(V3f)+microBufferSize));
}

void NonDiffusePointCloud::readSurphelFromFile(long startPointer) {

	ifstream myFile(filename.c_str(), ios::in | ios::binary);
	if (myFile.is_open()){
//		myFile.seekg(startPointer,ios::beg);
//		myFile.read(currentSurphel.getPositionPointer(), sizeof(V3f));
//		myFile.seekg(startPointer+sizeof(V3f),ios::beg);
//		myFile.read(currentSurphel.getNormalPointer(), sizeof(V3f));
//		myFile.seekg(startPointer+2*sizeof(V3f),ios::beg);
//		myFile.read(currentSurphel.getPixelPointer(), microBufferSize);
		myFile.close();
	} else {
		  Aqsis::log() << warning << "Unable to read microbuffer from " << filename << std::endl;
	}
}

NonDiffusePointCloud::~NonDiffusePointCloud() {

}

}
