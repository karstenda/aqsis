/*
 * NonDiffusePoint.h
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#ifndef NONDIFFUSEPOINT2_H_
#define NONDIFFUSEPOINT2_H_

#include <aqsis/util/logging.h>


#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>
#include <OpenEXR/ImathMath.h>
#include <OpenEXR/ImathFun.h>

#include <boost/scoped_array.hpp>

#include "../MicroBuf.h"

namespace Aqsis {

class NonDiffusePoint2 {

private:

	const float* data;
	int size;

public:

	NonDiffusePoint2(const float* data, int size);
	virtual ~NonDiffusePoint2();

	Imath::C3f getRadiosityInDir(Imath::V3f dir);

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
};

}
#endif /* NONDIFFUSEPOINT2_H_ */
