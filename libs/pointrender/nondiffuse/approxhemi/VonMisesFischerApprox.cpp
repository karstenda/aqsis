/*
 * VonMisesFischerApprox.cpp
 *
 *  Created on: May 19, 2013
 *      Author: karsten
 */

#include <limits>
#include "VonMisesFischerApprox.h"
#include <aqsis/util/logging.h>


namespace Aqsis {

using Imath::V3f;
using Imath::C3f;
using std::vector;


VonMisesFischerApprox::VonMisesFischerApprox(int nLobes) :
		kappas(nLobes,0), mus(nLobes,V3f(0,0,0)), radiosities(nLobes,C3f(0,0,0))  {

}

VonMisesFischerApprox::VonMisesFischerApprox(const float* data, int length):
		kappas(), mus(),  radiosities() {
	int nLobes = (length-1)/7;

	kappas.resize(nLobes,0.0f);
	mus.resize(nLobes,V3f(0,0,0));
	radiosities.resize(nLobes,C3f(0,0,0));

	for (int i=0; i < nLobes; i++) {
		kappas[i] = data[i*7+1];
		mus[i] = V3f(data[(i*7+2)],data[(i*7+3)],data[(i*7+4)]);
		radiosities[i] = C3f(data[(i*7+5)],data[(i*7+6)],data[(i*7+7)]);
	}
}


V3f applyDeriv(V3f N, V3f d) {

	float dotp = d ^ d;

	if (dotp != 0) {
		V3f N2 = (N - ((d ^ N)/(dotp) * d));
		N2.normalize();
		return N2;
	} else {
		return N;
	}
}

inline float dot(V3f a, V3f b) {
	return a ^ b;
}

void VonMisesFischerApprox::approximate(const Hemisphere& hemi) {

	float kappa = hemi.getPhong();
	V3f N = hemi.getNormal();
	V3f* directions = hemi.getLobeDirections();
	C3f* hemiRads = hemi.getLobeRadiosities();

	for (int i=0; i <mus.size(); i++) {
		if (i >= hemi.getNLobes()) {
			float nan = std::numeric_limits<float>::quiet_NaN();
			mus[i] = V3f(nan,nan,nan);
		} else {
			V3f L = directions[i];
			V3f R = -L - 2 * (dot(-L, N)) * N;

			mus[i] = R;
			radiosities[i] = hemiRads[i]*dot(L,N);
			kappas[i] = kappa;
		}
	}
}

C3f VonMisesFischerApprox::getRadiosityInDir(const Imath::V3f dir) {
	C3f tot(0, 0, 0);
	for (int i = 0; i < mus.size(); i++) {
		if (!isnan(mus[i].x)) {

			V3f Cl = radiosities[i];
			V3f R = mus[i];
			float kappa = kappas[i];
			float normR = 1 - 1/kappa;

			float dotp = dir ^ R;
			if (dotp > 0) {
				float intensity = (kappa/(2*M_PI))*exp((dotp-1)*kappa);
				tot += Cl * intensity;
			}
		}
	}
	return tot;
}

C3f VonMisesFischerApprox::getRadiosityInDir(const Imath::V3f dir, float distRatio) {

	C3f tot(0, 0, 0);


	for (int i = 0; i < mus.size(); i++) {
		if (!isnan(mus[i].x)) {

			V3f Cl = radiosities[i];
			V3f R = mus[i];
			float kappa = kappas[i];
			float normR = 1 - 1/kappa;

			float dotp = dir ^ R;
			if (dotp > 0) {

				// Behold! My soon to be patented distRatio function in action!
				float alpha;
				if (isinf(distRatio)) {
					alpha = 1;
				} else {
					 float beta1 = 3.986420112227081/kappa;
					 float beta2 = 0.121681812302997*(kappa/(17.0393173526303+kappa))+-0.128951522126267;
					 alpha = 1-(1/(beta1*distRatio*distRatio+beta2*distRatio+1));
				}

				kappa = alpha*kappa;
				float intensity = (kappa/(2*M_PI))*exp((dotp-1)*kappa);
				tot += Cl * intensity;
			}
		}
	}
	return tot;
}

int VonMisesFischerApprox::getFloatArraySize() {
	return mus.size()*7+1;
}

int VonMisesFischerApprox::writeToFloatArray(float* data) {
	data[0] = (float) HemiApprox::VonMisesFischer;
	for (int i=0; i < mus.size(); i++) {
		data[i*7+1] = kappas[i];
		data[i*7+2] = mus[i].x;
		data[i*7+3] = mus[i].y;
		data[i*7+4] = mus[i].z;
		data[i*7+5] = radiosities[i].x;
		data[i*7+6] = radiosities[i].y;
		data[i*7+7] = radiosities[i].z;
	}
}

void VonMisesFischerApprox::add(const HemiApprox* other) {
	const VonMisesFischerApprox* otherc = dynamic_cast<const VonMisesFischerApprox*>(other);
	if (otherc) {
		*this += *otherc;
	} else {
		Aqsis::log() << error << "Can not add two different types of HemiApprox together (PhongModelApprox)." << std::endl;
	}
}

VonMisesFischerApprox& VonMisesFischerApprox::operator+= ( const VonMisesFischerApprox& other) {

	// TODO

	return *this;
}


HemiApprox::Type VonMisesFischerApprox::getType() {
	return HemiApprox::VonMisesFischer;
}

int VonMisesFischerApprox::calculateFloatArraySize(int nLobes) {
	return nLobes*7+1;
}

HemiApprox* VonMisesFischerApprox::getDarkApprox() {
	return new VonMisesFischerApprox(0);
}





}
