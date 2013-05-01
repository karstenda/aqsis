/*
 * Hemisphere.cpp
 *
 *  Created on: Mar 5, 2013
 *      Author: karsten
 */

#include <aqsis/util/logging.h>

#include "Hemisphere.h"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;


inline float dot(V3f a, V3f b) {
	return a ^ b;
}

Hemisphere::Hemisphere( V3f N, int phong, V3f* directions, C3f* radiances, int num)
		: N(N), phong(phong), directions(directions), radiances(radiances), num(num) {
	}


float Hemisphere::operator () (double theta, double phi) const {
	return radiosityInSpherDir(theta,phi).x;
}

C3f Hemisphere::radiosityInSpherDir(double theta, double phi) const {

	V3f dir = sphToCart(V3f(theta, phi, 1));
	return radiosityInDir(dir);

}

C3f Hemisphere::radiosityInDir(const V3f dir) const {

	C3f tot(0, 0, 0);

	if (num == 0)
		return tot;

	for (int i = 0; i < num; i++) {
		V3f L = directions[i];
		V3f Cl = radiances[i];
		V3f R = -L - 2 * (dot(-L, N)) * N;

		float dotp = dot(dir, N);
		if (dotp > 0) {
			float phongFactor = pow(std::max(0.0f, dot(R, dir)), phong);
			float normPhongFactor = phongFactor * ((phong + 1) / (2 * M_PI));
			tot += Cl * dotp * normPhongFactor;
		}

//		if (isnan(tot.x) || isnan(tot.y) || isnan(tot.z)) {
//			Aqsis::log() << warning << "NaN-> L:(" << L.x << ", " << L.y
//					<< ", " << L.z << ") Cl:(" << Cl.x << ", " << Cl.y << ", "
//					<< Cl.z << ") num: " << num << std::endl;
//			Aqsis::log() << warning << "-" << std::endl;
//		}
	}
	return tot;
}


Hemisphere::~Hemisphere() {
}


V3f cartToSph(const V3f dir) {
	V3f tmp;
	tmp.z = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	tmp.x = acos(dir.z / tmp.z);
	if ( 0 <= dir.x) {
		tmp.y = atan(dir.y / dir.x);
	} else {
		tmp.y = M_PI-atan(dir.y / dir.x);
	}
	return tmp;
}

V3f sphToCart(const V3f dir) {
	V3f tmp;
	tmp.z = dir.z * cos(dir.x);
	tmp.x = dir.z * sin(dir.x)*cos(dir.y);
	tmp.y = dir.z * sin(dir.x)*sin(dir.y);
	return tmp;
}


}
