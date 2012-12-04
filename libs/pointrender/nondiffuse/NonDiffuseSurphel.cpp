/*
 * NonDiffuseSurphel.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: karsten
 */

#include	<aqsis/aqsis.h>


#include	"../microbuffer.h"
#include "NonDiffuseSurphel.hpp"

namespace Aqsis {

/**
 * Microbuffer has to be reference of a microbuffer on the heap.
 */

NonDiffuseSurphel::NonDiffuseSurphel(int faceres) :
	position(),
	normal(),
	radius(0),
	phong(20),
	integrator(faceres) {

}


NonDiffuseSurphel::NonDiffuseSurphel(	V3f position,
											V3f normal,
											float radius,
											int phong,
											int faceRes,
											float* rawPixelData) :
		position(position),
		normal(normal),
		radius(radius),
		phong(phong),
		integrator(faceRes) {

	integrator.microBuf().reset(rawPixelData);
}

C3f NonDiffuseSurphel::getRadiosity(V3f direction) {
	float occ = 0;
	return integrator.phongRadiosity(normal,direction,phong,&occ);
}

float* NonDiffuseSurphel::getRadiusPointer() {
	return &radius;
}

int* NonDiffuseSurphel::getPhongPointer() {
	return &phong;
}

float* NonDiffuseSurphel::getPixelPointer() {
	return integrator.microBuf().getPixelPointer();
}

V3f* NonDiffuseSurphel::getNormalPointer() {
	return &normal;
}
V3f* NonDiffuseSurphel::getPositionPointer() {
	return &position;
}

NonDiffuseSurphel::~NonDiffuseSurphel() {
	// TODO Auto-generated destructor stub
}

}
