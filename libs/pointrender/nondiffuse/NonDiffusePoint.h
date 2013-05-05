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
#include "approxhemi/HemiApprox.h"

namespace Aqsis {


//HemiApprox* toHemiApprox(float* data, int length) {
//
//	HemiApprox::Type type = (HemiApprox::Type) data[0];
//
//	switch(type) {
//
//		case HemiApprox::MicroBuf:
//			return new MicroBufApprox(data,length);
//
//		case HemiApprox::SpherHarmon:
//			return new SpherHarmonApprox(data,length);
//	}
//}

class NonDiffusePoint {

private:

	Imath::V3f position;
	Imath::V3f normal;
	float radius;
	HemiApprox* hemiApprox;

public:

	NonDiffusePoint();

	NonDiffusePoint(Imath::V3f position, Imath::V3f normal, float radius, HemiApprox* hemiApprox);

	~NonDiffusePoint();

	float getRadius() const;

	Imath::V3f getPosition() const;

	Imath::V3f getNormal() const;

	HemiApprox* getHemi() const;

};

}
#endif /* NONDIFFUSEPOINT_H_ */
