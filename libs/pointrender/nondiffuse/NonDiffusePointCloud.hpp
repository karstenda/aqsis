/*
 * NonDiffusePointCloud.h
 *
 *  Created on: Nov 2, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINTCLOUD_HPP_
#define NONDIFFUSEPOINTCLOUD_HPP_


#include	"NonDiffuseSurphel.hpp"


namespace Aqsis {

class NonDiffusePointCloud {

private:

	std::string filename;
	int microBufferSize;
	int currentSurphelIndex;
	NonDiffuseSurphel currentSurphel;

public:

	NonDiffusePointCloud(std::string filename,int faceRes, int nchans);
	virtual ~NonDiffusePointCloud();
	void addSurphel(V3f position, V3f normal, int phong, float* pixels);
	bool isValid();

	void setCurrentSurphelIndex(int surphelIndex);
	NonDiffuseSurphel* getCurrentSurphel();

private:

	void writeToFile(V3f position, V3f normal, int phong, float* pixels);
	void readSurphelFromFile(long startPointer);

};

}
#endif /* NONDIFFUSEPOINTCLOUD_H_ */
