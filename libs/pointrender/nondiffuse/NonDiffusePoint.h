/*
 * NonDiffusePoint.h
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINT_H_
#define NONDIFFUSEPOINT_H_

#include <aqsis/util/logging.h>


#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>
#include <OpenEXR/ImathMath.h>
#include <OpenEXR/ImathFun.h>

#include <boost/scoped_array.hpp>

#include "../MicroBuf.h"

namespace Aqsis {

class NonDiffusePoint {

private:

	static int cachedRes;
	static boost::scoped_array<float> cachedPixelSizes;

	const float* data;
	int faceRes;

public:

	NonDiffusePoint(const float* data, int size);
	virtual ~NonDiffusePoint();

	Imath::C3f getRadiosityInDir(Imath::V3f dir);
	Imath::C3f getInterpolatedRadiosityInDir(Imath::V3f dir);
	Imath::C3f getInterpolatedRadiosityInDir2(Imath::V3f dir);

	float getRadius() {
		return *(data+6);
	}

	Imath::V3f getPosition() {
		return *(Imath::V3f*) data;
	}

	Imath::V3f getNormal() {
		return *(Imath::V3f*) (data+3);
	}

	inline const float* getHemi() {
		return data+7;
	}

	inline const float* getPixel(int index) {
		return getHemi()+index*3;
	}

	static int getHemiRes(int size) {
		int hemiSize = size - 7;
		float res = sqrt(hemiSize/(6*3));
		int intres1 = floor(res);
		int intres2 = ceil(res);
		if (intres1 != intres2) {
			Aqsis::log() << error << "Invalid hemisphere resolution in loaded pointfile: "<< res << std::endl;
		}
		return intres1;
	}

	const float* getHemiFace(int f);


	void writeMicroBufImage(std::string filename);

	static float getPixelSize(int res, int u, int v);
};

}
#endif /* NONDIFFUSEPOINT_H_ */
