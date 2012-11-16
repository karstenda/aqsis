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
		faceRes(faceRes),
		nchans(nchans),
		nFloatsMicroBuffer(MicroBuf::Face_end*nchans*faceRes*faceRes),
		microBufferSize(nFloatsMicroBuffer*sizeof(float)),
		memblock(new float[nFloatsMicroBuffer]) {

}

void NonDiffusePointCloud::addPoint(V3f position, V3f normal, float* pixels) {
	writeToFile(position, normal, pixels, microBufferSize);
}

void NonDiffusePointCloud::writeToFile(V3f position, V3f normal, float* pixels, int length) {

	ofstream myFile(filename.c_str(), ios::out | ios::binary | ios::app);
	if (myFile.is_open()){
		myFile.write ((char*) &position, sizeof(V3f));
		myFile.write ((char*) &normal, sizeof(V3f));
		myFile.write ((char*) pixels, length);
		myFile.close();
	} else {
		Aqsis::log() << warning << "Unable to write microbuffer to " << filename << std::endl;
	}
}


float* const NonDiffusePointCloud::readFromFile(long startPointer, int length) {

	ifstream myFile(filename.c_str(), ios::in | ios::binary);
	if (myFile.is_open()){
		myFile.seekg(startPointer,ios::beg);
		myFile.read((char*) memblock, length+2*sizeof(V3f));
		myFile.close();
	} else {
		  Aqsis::log() << warning << "Unable to read microbuffer from " << filename << std::endl;
	}
	return memblock;
}

NonDiffusePointCloud::~NonDiffusePointCloud() {
	delete[] memblock;
}

}
