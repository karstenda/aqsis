/*
 * NonDiffuseSurphel.h
 *
 *  Created on: Nov 6, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSESURPHEL_H_
#define NONDIFFUSESURPHEL_H_

namespace Aqsis {


class NonDiffuseSurphel {

private:

	V3f position;
	V3f normal;
	const float* pixels;
	MicroBuf& microbuffer;

public:
	NonDiffuseSurphel(V3f position, V3f normal, const float* pixels, MicroBuf& microbuffer);

	C3f getRadiosity(V3f direction);

	virtual ~NonDiffuseSurphel();
};

}

#endif /* NONDIFFUSESURPHEL_H_ */
