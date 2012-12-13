/*
 * NonDiffusePointCloud.h
 *
 *  Created on: Nov 2, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSEFILEHANDLER_HPP_
#define NONDIFFUSEFILEHANDLER_HPP_

#include	<fstream>

#include	"NonDiffuseSurphel.hpp"


namespace Aqsis {

class NonDiffuseFileHandler {

private:

	std::string filename;
	int microBufferSize;
	int surphelSize;
	int faceRes;
	long nSurphels;
	std::fstream filehandle;


public:

	/**
	 * This constructor is used to create a new NonDiffusePointCloud. If the file alread exists,
	 * it will be overwritten.
	 */
	NonDiffuseFileHandler(std::string filename,int faceRes, int nchans);

	/**
	 * This constructor is used to load an existing NonDiffusePointCloud from a file.
	 */
	NonDiffuseFileHandler(std::string filename);
	virtual ~NonDiffuseFileHandler();

	void addNonDiffuseSurphel(Imath::V3f position, Imath::V3f normal, float radius, int phong, float* pixels);
	void loadNonDiffuseSurphels(int fromIndex, int size, std::vector<NonDiffuseSurphel>* surphelCache);
	int getSurphelSize();
	int getNSurphels();
	int getFaceRes();

};

}
#endif /* NONDIFFUSEPFILEHANDLER_HPP_ */
