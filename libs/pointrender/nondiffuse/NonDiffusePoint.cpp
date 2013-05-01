/*
 * NonDiffusePoint.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#include "NonDiffusePoint.h"

#include "../../../thirdparty/pngpp/png.hpp"


namespace Aqsis {

using Imath::C3f;
using Imath::V3f;

NonDiffusePoint::NonDiffusePoint():
		position(), normal(), radius(), hemiApprox(NULL){
}

NonDiffusePoint::NonDiffusePoint(V3f position, V3f normal,
		float radius, HemiApprox* hemiApprox) :
	position(position), normal(normal), radius(radius), hemiApprox(hemiApprox) {
}

V3f NonDiffusePoint::getPosition() const {
	return position;
}

V3f NonDiffusePoint::getNormal() const {
	return normal;
}

float NonDiffusePoint::getRadius() const {
	return radius;
}

HemiApprox* NonDiffusePoint::getHemi() const {
	return hemiApprox;
}

NonDiffusePoint::~NonDiffusePoint() {
//	delete hemiApprox;
}


}
