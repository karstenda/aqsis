/*
 * SpherHarmonApprox.h
 *
 *  Created on: Mar 19, 2013
 *      Author: karsten
 */

#ifndef SPHERHARMONAPPROX_H_
#define SPHERHARMONAPPROX_H_

#include "HemiApprox.h"
#include "../Hemisphere.h"
#include "../SHProjection.h"

namespace Aqsis {

class SpherHarmonApprox: public HemiApprox {

public:
	static int nApproximationSamples;
private:
	SHProjection<float> shApprox;


public:

	SpherHarmonApprox(int nBands);

	SpherHarmonApprox(const float* data, int length);

	void approximate(const Hemisphere& hemi);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction);

	int getFloatArraySize();

	int writeToFloatArray(float* data);

	HemiApprox::Type getType();

	static int calculateFloatArraySize(int nBands);

	void add(const HemiApprox* other);

	SpherHarmonApprox& operator+= ( const SpherHarmonApprox& other);

	HemiApprox* getDarkEquivalent();

	~SpherHarmonApprox() {}
};

}
#endif /* SPHERHARMONAPPROX_H_ */
