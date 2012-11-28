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

NonDiffuseSurphel::NonDiffuseSurphel(int faceres, int nchans) :
	position(),
	normal(),
	microbuffer(faceres, nchans, {0}) {

}


NonDiffuseSurphel::NonDiffuseSurphel(V3f position, V3f normal, MicroBuf micro) :
		position(position),
		normal(normal),
		microbuffer(micro.getFaceResolution(),micro.getNChans(),{0}) {
	microbuffer.reset(micro.getRawPixelData());
}

C3f NonDiffuseSurphel::getRadiosity(V3f direction) {

}

float* NonDiffuseSurphel::getPixelPointer() {
	return microbuffer.getPixelPointer();
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
