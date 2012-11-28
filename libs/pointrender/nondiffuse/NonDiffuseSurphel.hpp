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
	MicroBuf microbuffer;

public:

	NonDiffuseSurphel(int faceres, int nchans);
	NonDiffuseSurphel(V3f position, V3f normal, MicroBuf microbuffer);

	C3f getRadiosity(V3f direction);

	float* getPixelPointer();
	V3f* getNormalPointer();
	V3f* getPositionPointer();

	virtual ~NonDiffuseSurphel();
};

}

#endif /* NONDIFFUSESURPHEL_H_ */
