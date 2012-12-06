/*
 * NonDiffuseFileHandler.cpp
 *
 *  Created on: Dec 4, 2012
 *      Author: Karsten
 */


#include	<string>
#include	<sys/stat.h>
#include	<sys/types.h>

#include	<aqsis/aqsis.h>
#include	<aqsis/util/logging.h>


#include	"../microbuffer.h"
#include	"NonDiffuseFileHandler.hpp"


using namespace std;

namespace Aqsis {

/**
 * This constructor is used to create a new NonDiffusePointCloud. If the file alread exists,
 * it will be overwritten.
 */
NonDiffuseFileHandler::NonDiffuseFileHandler(
		const string filename,
		const int faceRes,
		int nchans):

		filename(filename),
		microBufferSize(MicroBuf::Face_end*nchans*faceRes*faceRes*sizeof(float)),
		surphelSize(microBufferSize+2*sizeof(V3f)+sizeof(int)+sizeof(float)),
		faceRes(faceRes),
		nSurphels(0),
		filehandle(filename.c_str(),  ios::out  | ios::in | ios::binary | ios::trunc) {

	if (filehandle.is_open()){
		filehandle.write((char*) &faceRes, sizeof(int));
		filehandle.write((char*) &nchans, sizeof(int));
	} else {
		Aqsis::log() << error << "Unable to create " << filename << std::endl;
		exit(1);
	}
	Aqsis::log() << warning << "Created " << filename << std::endl;

}

/**
 * This constructor is used to load an existing NonDiffusePointCloud from a file.
 */
NonDiffuseFileHandler::NonDiffuseFileHandler(const string filename):
		filename(filename),
		microBufferSize(0),
		surphelSize(0),
		faceRes(faceRes),
		nSurphels(0),
		filehandle(filename.c_str(),  ios::out  | ios::in | ios::binary ) {

	if (filehandle.is_open()) {

		// How many surphels are there in the file?
		filehandle.seekg(0,ios::end);
		int surphelDataSize = filehandle.tellg() - 2*sizeof(int);

		// Load the header vars from the file.
		int nchans = 5;
		filehandle.seekp(0,ios::beg);
		filehandle.read((char*) &faceRes, sizeof(int));
		filehandle.read((char*) &nchans, sizeof(int));

		// calculate the class vars.
		microBufferSize = MicroBuf::Face_end*nchans*faceRes*faceRes*sizeof(float);
		surphelSize = microBufferSize+2*sizeof(V3f)+sizeof(int)+sizeof(float);
		nSurphels = surphelDataSize/surphelSize;

	} else {
		Aqsis::log() << error << "Unable to open " << filename << std::endl;
		exit(1);
	}
	Aqsis::log() << warning << "Opened " << filename << " containing " << nSurphels << " surphels." << std::endl;
}



void NonDiffuseFileHandler::addNonDiffuseSurphel(
		V3f position,
		V3f normal,
		float radius,
		int phong,
		float* pixels) {

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

int NonDiffuseFileHandler::getNSurphels() {
	return nSurphels;
}

int NonDiffuseFileHandler::getFaceRes() {
	return faceRes;
}


void NonDiffuseFileHandler::loadNonDiffuseSurphels(	const int fromIndex,
															const int size,
															vector<NonDiffuseSurphel>* surphelCache) {

	if (fromIndex+size > nSurphels) {
		Aqsis::log() << error << "Can't load " << size << " surphels from index "<< fromIndex
				<<", there are only "<< nSurphels<<" surphels in the file."<<std::endl;
		exit(1);
	}

	if (filehandle.is_open()){

		// Go with the read pointer to the beginning of the file (after the header)
		filehandle.seekg(2*sizeof(int)+fromIndex*surphelSize,ios::beg);

		// Load all the surphels in the array
		for (int i=0;i<size;i++) {
			filehandle.read((char*) surphelCache->at(i).getPositionPointer(), sizeof(V3f));
			filehandle.read((char*) surphelCache->at(i).getNormalPointer(), sizeof(V3f));
			filehandle.read((char*) surphelCache->at(i).getRadiusPointer(), sizeof(float));
			filehandle.read((char*) surphelCache->at(i).getPhongPointer(), sizeof(int));
			filehandle.read((char*) surphelCache->at(i).getPixelPointer(), microBufferSize);
		}
	} else {
		  Aqsis::log() << error << "Unable to load NonDiffusePointCloud from " << filename << std::endl;
		  exit(1);
	}

}

NonDiffuseFileHandler::~NonDiffuseFileHandler() {
	filehandle.close();
}

}
