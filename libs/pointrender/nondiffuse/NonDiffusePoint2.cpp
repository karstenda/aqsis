/*
 * NonDiffusePoint2.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#include "NonDiffusePoint2.h"


namespace Aqsis {

using Imath::C3f;
using Imath::V3f;

NonDiffusePoint2::NonDiffusePoint2(const float* data, int size):
		data(data),size(size) {
}

NonDiffusePoint2::~NonDiffusePoint2() {

}

Imath::C3f getRadiosityInDir(Imath::V3f dir) {
	C3f temp;
	return temp;
}


}
