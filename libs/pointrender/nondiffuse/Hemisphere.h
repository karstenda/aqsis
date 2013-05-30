/*
 * Hemisphere.h
 *
 *  Created on: Mar 5, 2013
 *      Author: karsten
 */

#ifndef HEMISPHERE_H_
#define HEMISPHERE_H_

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>
#include <OpenEXR/ImathMath.h>
#include <OpenEXR/ImathFun.h>

namespace Aqsis {

class Hemisphere {

private:

	Imath::V3f N;
	int phong;
	Imath::V3f* directions;
	Imath::C3f* radiances;
	const int num;
	Imath::V3f dPu;
	Imath::V3f dPv;
	float radius;

public:
	Hemisphere( Imath::V3f N, int phong, Imath::V3f* directions, Imath::C3f* radiances, int num);

	~Hemisphere();

	Imath::C3f radiosityInSpherDir (double theta, double phi) const;

	Imath::C3f radiosityInDir (const Imath::V3f dir) const;

	float operator () (double theta, double phi) const;

	Imath::V3f getNormal() const {
		return N;
	}

	Imath::V3f* getLobeDirections() const {
		return directions;
	}

	Imath::C3f* getLobeRadiosities() const {
		return radiances;
	}

	int getNLobes() const {
		return num;
	}

	int getPhong() const {
		return phong;
	}

	Imath::V3f getdPdu() const {
		return dPu;
	}

	Imath::V3f getdPdv() const {
		return dPv;
	}

	void setdPdv(Imath::V3f dPvVec) {
		this->dPv = dPvVec;
	}

	void setdPdu(Imath::V3f dPuVec) {
		this->dPu = dPuVec;
	}

	float getRadius() const {
		return radius;
	}

	void setRadius(float radius) {
		this->radius = radius;
	}
};


Imath::V3f cartToSph(const Imath::V3f dir);

Imath::V3f sphToCart(const Imath::V3f dir);

}

#endif /* HEMISPHERE_H_ */
