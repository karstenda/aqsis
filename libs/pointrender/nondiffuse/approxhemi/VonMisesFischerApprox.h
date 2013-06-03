/*
 * VonMisesFischerApprox.h
 *
 *  Created on: May 19, 2013
 *      Author: karsten
 */

#ifndef VONMISESFISCHERAPPROX_H_
#define VONMISESFISCHERAPPROX_H_

#include "HemiApprox.h"
#include "../Hemisphere.h"

#include <vector>

namespace Aqsis {

class VonMisesFischerApprox: public HemiApprox {

private:

	std::vector<float> kappas;
	std::vector<Imath::V3f> mus;
	std::vector<Imath::C3f> radiosities;

public:

	VonMisesFischerApprox(int nLobes);

	VonMisesFischerApprox(const float* data, int length);

	void approximate(const Hemisphere& hemi);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction, float distRatio);

	int getFloatArraySize();

	int writeToFloatArray(float* data);

	HemiApprox::Type getType();

	static int calculateFloatArraySize(int nBands);

	void add(const HemiApprox* other);

	VonMisesFischerApprox& operator+= ( const VonMisesFischerApprox& other);

	HemiApprox* getDarkApprox();

	~VonMisesFischerApprox() {}
};

}

#endif /* VONMISESFISCHERAPPROX_H_ */
