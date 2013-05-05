/*
 * HemisApprox.h
 *
 *  Created on: Mar 19, 2013
 *      Author: karsten
 */

#ifndef HEMIAPPROX_H_
#define HEMIAPPROX_H_

#include <aqsis/aqsis.h>
#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathColor.h>
#include "../Hemisphere.h"

namespace Aqsis {

class HemiApprox {

public:

	enum Type {
		SpherHarmon = 0, //   0
		CubeMap = 1,     //   1
		PhongModel = 2,  //   2
	};

	virtual Type getType() = 0;

	virtual Imath::C3f getRadiosityInDir(const Imath::V3f direction) = 0;

	virtual void approximate(const Hemisphere& hemi) = 0;

	virtual int getFloatArraySize() = 0;

	virtual int writeToFloatArray(float* data) = 0;

	virtual ~HemiApprox() {}

};

}

#endif /* HEMIAPPROX_H_ */
