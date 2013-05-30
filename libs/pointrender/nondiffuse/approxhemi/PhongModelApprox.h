/*
 * PhongModelApprox.h
 *
 *  Created on: May 3, 2013
 *      Author: karsten
 */

#ifndef PHONGMODELAPPROX_H_
#define PHONGMODELAPPROX_H_

#include "HemiApprox.h"
#include "../Hemisphere.h"

#include <vector>

namespace Aqsis {

class PhongModelApprox: public HemiApprox {

private:
	int phong;
	Imath::V3f N;
	std::vector<Imath::V3f> lobeDirs;
	std::vector<Imath::C3f> lobeCols;

public:

	PhongModelApprox(int nLobes);

	PhongModelApprox(const float* data, int length);

	void approximate(const Hemisphere& hemi);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction, float distRatio);

	int getFloatArraySize();

	int writeToFloatArray(float* data);

	HemiApprox::Type getType();

	static int calculateFloatArraySize(int nBands);

	void add(const HemiApprox* other);

	PhongModelApprox& operator+= ( const PhongModelApprox& other);

	HemiApprox* getDarkApprox();

	Imath::V3f getNormal() const {
		return N;
	}

	~PhongModelApprox() {}
};

}
#endif /* SPHERHARMONAPPROX_H_ */
