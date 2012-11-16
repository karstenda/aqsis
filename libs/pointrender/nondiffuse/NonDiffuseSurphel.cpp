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


NonDiffuseSurphel::NonDiffuseSurphel(V3f position, V3f normal, const float* pixels, MicroBuf& microbuffer) :
		position(position),
		normal(normal),
		pixels(pixels),
		microbuffer(microbuffer) {


}

C3f NonDiffuseSurphel::getRadiosity(V3f direction) {
	microbuffer.reset(pixels);
}


NonDiffuseSurphel::~NonDiffuseSurphel() {
	// TODO Auto-generated destructor stub
}

}
