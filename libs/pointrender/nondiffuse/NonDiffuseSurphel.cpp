/*
 * NonDiffuseSurphel.cpp
 *
 *  Created on: Nov 6, 2012
 *      Author: karsten
 */

#include	<aqsis/aqsis.h>


#include	"../RadiosityIntegrator.h"
#include "NonDiffuseSurphel.hpp"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

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

NonDiffuseSurphel::NonDiffuseSurphel(const NonDiffuseSurphel& surphel) :
		position(surphel.position),
		normal(surphel.normal),
		radius(surphel.radius),
		phong(surphel.phong),
		integrator(surphel.integrator.res()){

}


C3f NonDiffuseSurphel::getRadiosity(const V3f direction) {
	V3f dir = direction;
	dir = dir.normalize();
	MicroBuf& microbuf = integrator.microBuf();
	return microbuf.getRadiosityInDir(dir);
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
