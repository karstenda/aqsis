/*
 * NonDiffusePointCloud.h
 *
 *  Created on: Nov 2, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINTCLOUD_HPP_
#define NONDIFFUSEPOINTCLOUD_HPP_

#include	<fstream>
#include	"NonDiffuseSurphel.hpp"


namespace Aqsis {

class NonDiffusePointCloud {

private:

	std::string filename;
	int microBufferSize;
	int surphelSize;
	int currentSurphelIndex;
	NonDiffuseSurphel currentSurphel;
	long nSurphels;
	std::fstream filehandle;


public:

	NonDiffusePointCloud(std::string filename,int faceRes, int nchans);
	NonDiffusePointCloud(std::string filename,int faceRes, int nchans, long surphelDataSize);
	virtual ~NonDiffusePointCloud();

	void addSurphel(V3f position, V3f normal, int phong, float* pixels);
	int getNSurphel();
	NonDiffuseSurphel* getSurphel(int surphelIndex);
	void setCurrentSurphelIndex(int surphelIndex);
	NonDiffuseSurphel* getNextSurphel();

private:

	void readCurrentSurphelFromFile();

};

}
#endif /* NONDIFFUSEPOINTCLOUD_H_ */
