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

namespace Aqsis {

class NonDiffusePoint {

private:
	float* position;
	float* normal;
	float* area;
	float* hemi;
	int hemiRes;

public:

	NonDiffusePoint(float* data, int size);
	virtual ~NonDiffusePoint();

	Imath::C3f getRadiosityInDir(Imath::V3f);
	float getArea();
	Imath::V3f getPosition();
	Imath::V3f getNormal();

	static int getHemiRes(int size) {
		int hemiSize = size - 7;
		float res = sqrt(hemiSize/6);
		int intres1 = floor(res);
		int intres2 = ceil(res);
		if (intres1 != intres2) {
			Aqsis::log() << error << "Invalid hemisphere resolution in loaded pointfile: "<< res << std::endl;
		}
		return intres1;
	}

};

}
#endif /* NONDIFFUSEPOINT_H_ */
