/*
 * PhongModelApprox.cpp
 *
 *  Created on: May 3, 2013
 *      Author: karsten
 */

#include	<limits>


#include "PhongModelApprox.h"
#include <aqsis/util/logging.h>


namespace Aqsis {

using Imath::V3f;
using Imath::C3f;
using std::vector;


PhongModelApprox::PhongModelApprox(int nLobes) :
		lobeDirs(nLobes,V3f(0,0,0)), lobeCols(nLobes,C3f(0,0,0))  {
}

PhongModelApprox::PhongModelApprox(const float* data, int length):
	lobeDirs(), lobeCols() {
	int nLobes = (length-5)/6;

	phong = (int) data[1];
	N = V3f(data[2],data[3],data[4]);
	lobeDirs.resize(nLobes,V3f(0,0,0));
	lobeCols.resize(nLobes,C3f(0,0,0));

	for (int i=0; i < nLobes; i++) {
		lobeDirs[i] = V3f(data[(i*6+5)],data[(i*6+6)],data[(i*6+7)]);
		lobeCols[i] = C3f(data[(i*6+8)],data[(i*6+9)],data[(i*6+10)]);
	}
}

inline float dot(V3f a, V3f b) {
	return a ^ b;
}

void PhongModelApprox::approximate(const Hemisphere& hemi) {

	N = hemi.getNormal();
	phong = hemi.getPhong();
	V3f* directions = hemi.getLobeDirections();
	C3f* radiosities = hemi.getLobeRadiosities();


	for (int i=0; i <lobeDirs.size(); i++) {
		if (i >= hemi.getNLobes()) {
			float nan = std::numeric_limits<float>::quiet_NaN();
			lobeDirs[i] = V3f(nan,nan,nan);
		} else {
			V3f L = directions[i];
			V3f R = -L - 2 * (dot(-L, N)) * N;

			lobeDirs[i] = R;
			lobeCols[i] = radiosities[i];
		}
	}
}

C3f PhongModelApprox::getRadiosityInDir(const Imath::V3f dir) {

	C3f tot(0, 0, 0);

	for (int i = 0; i < lobeDirs.size(); i++) {
		if (!isnan(lobeDirs[i].x)) {

			V3f Cl = lobeCols[i];
			V3f R = lobeDirs[i];

			float dotp = dot(dir, N);
			if (dotp > 0) {
				float phongRad = pow(std::max(0.0f, dot(R, dir)), phong);
				float normPhongRad = phongRad	* ((phong + 1) / (2 * M_PI));
				tot += Cl * normPhongRad;
			}
		} else {

		}
	}
	return tot;
}

int PhongModelApprox::getFloatArraySize() {
	return lobeDirs.size()*6+5;
}

int PhongModelApprox::writeToFloatArray(float* data) {
	data[0] = (float) HemiApprox::PhongModel;
	data[1] = (float) phong;
	data[2] = N.x;
	data[3] = N.y;
	data[4] = N.z;
	for (int i=0; i < lobeDirs.size(); i++) {
		data[i*6+5] = lobeDirs[i].x;
		data[i*6+6] = lobeDirs[i].y;
		data[i*6+7] = lobeDirs[i].z;
		data[i*6+8] = lobeCols[i].x;
		data[i*6+9] = lobeCols[i].y;
		data[i*6+10] = lobeCols[i].z;
	}
}

void PhongModelApprox::add(const HemiApprox* other) {
	const PhongModelApprox* otherc = dynamic_cast<const PhongModelApprox*>(other);
	if (otherc) {
		*this += *otherc;
	} else {
		Aqsis::log() << error << "Can not add two different types of HemiApprox together (PhongModelApprox)." << std::endl;
	}
}

PhongModelApprox& PhongModelApprox::operator+= ( const PhongModelApprox& other) {

	this->lobeCols.reserve(this->lobeCols.size()+other.lobeCols.size());
	this->lobeDirs.reserve(this->lobeDirs.size()+other.lobeDirs.size());

	for (int i; i < other.lobeDirs.size(); i++) {
		this->lobeDirs.push_back(other.lobeDirs[i]);
		this->lobeCols.push_back(other.lobeCols[i]);
	}

	return *this;
}


HemiApprox::Type PhongModelApprox::getType() {
	return HemiApprox::PhongModel;
}

int PhongModelApprox::calculateFloatArraySize(int nLobes) {
	return nLobes*6+5;
}

HemiApprox* PhongModelApprox::getDarkEquivalent() {
	return new PhongModelApprox(0);
}



}
