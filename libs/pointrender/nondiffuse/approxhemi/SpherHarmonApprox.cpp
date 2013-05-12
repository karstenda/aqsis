/*
 * SpherHarmonApprox.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: karsten
 */

#include "SpherHarmonApprox.h"

#include <string.h>
#include <aqsis/util/logging.h>
#include <vector>



namespace Aqsis {

using Imath::V3f;
using Imath::C3f;
using std::vector;

int SpherHarmonApprox::nApproximationSamples = 1000;

SpherHarmonApprox::SpherHarmonApprox(int numBands): shApprox(numBands) {

}

SpherHarmonApprox::SpherHarmonApprox(const float* data1, int length): shApprox(0) {
	shApprox.SetCoefficients(&data1[1],length-1);
}

float sigma(int n, int l) {
	if (l == 0) {
		return 1.0;
	} else {
		return (sin((M_PI*l)/n))/((M_PI*l)/n);
	}
}

void applySigma(const vector< float >& coeffs, vector< float >& newCoeffs) {
	int n = sqrt(coeffs.size());
	int i = 0;
	for(int l=0; l<n; l++) {
		float sigmav = sigma(n,l);
		for (int m=-l; m<=l; ++m) {
			newCoeffs[i] = sigmav*coeffs[ l*(l+1) + m ];
			i++;
		}
	}
}

void SpherHarmonApprox::approximate(const Hemisphere& hemi) {
	shApprox.project(hemi,shApprox.GetNumBands(),nApproximationSamples, SHProjection<float>::IntegrationMethod_Simpson);
	vector<float> coeffs = shApprox.GetProjection();
	vector<float> newCoeffs(coeffs.size());
	applySigma(coeffs,newCoeffs);
	shApprox.SetCoefficients(newCoeffs);
}

C3f SpherHarmonApprox::getRadiosityInDir(const V3f direction) {
	float radCol = shApprox.Evaluate(direction.x,direction.y,direction.z);
	return C3f(radCol,radCol,radCol);
}

int SpherHarmonApprox::getFloatArraySize() {
	return shApprox.GetNumBands()*shApprox.GetNumBands()+1;
}

int SpherHarmonApprox::writeToFloatArray(float* data1) {
	data1[0] = (float) HemiApprox::SpherHarmon;
	vector<float> coeffs = shApprox.GetProjection();
	for (int i=0; i < coeffs.size(); i++) {
		data1[i+1] = coeffs[i];
	}
	return getFloatArraySize();
}

int SpherHarmonApprox::calculateFloatArraySize(int nBands) {
	return nBands*nBands+1;
}

void SpherHarmonApprox::add(const HemiApprox* other) {
	const SpherHarmonApprox* otherc = dynamic_cast<const SpherHarmonApprox*>(other);
	if (otherc) {
		*this += *otherc;
	} else {
		Aqsis::log() << error << "Can not add two different types of HemiApprox together (SpherHarmonApprox)." << std::endl;
	}
}

SpherHarmonApprox& SpherHarmonApprox::operator+= (const SpherHarmonApprox& other) {
	this->shApprox += other.shApprox;
	return *this;
}


HemiApprox::Type SpherHarmonApprox::getType() {
	return HemiApprox::SpherHarmon;
}

HemiApprox* SpherHarmonApprox::getDarkEquivalent() {
	return new SpherHarmonApprox(shApprox.GetNumBands());
}


}
