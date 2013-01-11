/*
 * NonDiffusePointOctree.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#include "NonDiffusePoint.h"

namespace Aqsis {

NonDiffusePoint::NonDiffusePoint(float* data, int hemires):
		position(data),normal(data+3),area(data+6),hemi(data+7),hemiRes(hemiRes) {

}

NonDiffusePoint::~NonDiffusePoint() {

}


}
