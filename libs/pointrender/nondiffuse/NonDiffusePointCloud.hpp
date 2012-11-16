/*
 * NonDiffusePointCloud.h
 *
 *  Created on: Nov 2, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINTCLOUD_HPP_
#define NONDIFFUSEPOINTCLOUD_HPP_

namespace Aqsis {

class NonDiffusePointCloud {

private:

	std::string filename;
	int faceRes;
	int nchans;
	int nFloatsMicroBuffer;
	int microBufferSize;
	float* const memblock;

public:

	NonDiffusePointCloud(std::string filename,int faceRes, int nchans);
	virtual ~NonDiffusePointCloud();
	void addPoint(V3f position, V3f normal, float* pixels);


private:

	void writeToFile(V3f position, V3f normal, float* pixels, int length);
	float* const readFromFile(long startPointer, int length);

};

}
#endif /* NONDIFFUSEPOINTCLOUD_H_ */
