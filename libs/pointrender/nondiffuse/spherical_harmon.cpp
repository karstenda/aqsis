/*
 * spherical_harmon.cpp
 *
 *  Created on: Feb 27, 2013
 *      Author: karsten
 *
 *
 *  This code is based on the code and instructions found in:
 *  http://www.research.scea.com/gdc2003/spherical-harmonic-lighting.pdf
 *
 */

#include <aqsis/util/logging.h>
#include <boost/math/special_functions/spherical_harmonic.hpp>

//#include "/usr/include/boost/math/special_functions/spheric_harmonic.hpp"


#include "spherical_harmon.h"



namespace Aqsis {

using Imath::C3f;
using Imath::V3f;

float factorials[35] = { 0.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0,
		40320.0, 362880.0, 3628800.0, 39916800.0, 479001600.0, 6227020800.0,
		87178291200.0, 1307674368000.0, 20922789888000.0, 355687428096000.0,
		6402373705728000.0, 121645100408832000.0, 2432902008176640000.0,
		51090942171709440000.0, 1124000727777607680000.0,
		25852016738884976640000.0, 620448401733239439360000.0,
		15511210043330985984000000.0, 403291461126605635584000000.0,
		10888869450418352160768000000.0, 304888344611713860501504000000.0,
		8841761993739701954543616000000.0, 265252859812191058636308480000000.0,
		8222838654177922817725562880000000.0,
		263130836933693530167218012160000000.0,
		8683317618811886495518194401280000000.0,
		295232799039604140847618609643520000000.0 };

/**
 * Calculate n!
 */
float factorial(int n) {
	if (n >= 35) {
		return n * factorial(n - 1);
	} else {
		return factorials[n];
	}
}

/**
 * evaluate an Associated Legendre Polynomial P(l,m,x) at x
 */
float P(int l, int m, float x) {
	float pmm = 1.0;
	if (m > 0) {
		float somx2 = sqrt((1.0 - x) * (1.0 + x));
		float fact = 1.0;
		for (int i = 1; i <= m; i++) {
			pmm *= (-fact) * somx2;
			fact += 2.0;
		}
	}

	if (l == m) {
		return pmm;
	}
	float pmmp1 = x * (2.0 * m + 1.0) * pmm;
	if (l == m + 1) {
		return pmmp1;
	}
	float pll = 0.0;
	for (int ll = m + 2; ll <= l; ++ll) {
		pll = ((2.0 * ll - 1.0) * x * pmmp1 - (ll + m - 1.0) * pmm) / (ll - m);
		pmm = pmmp1;
		pmmp1 = pll;
	}
	return pll;
}

/**
 * Renormalisation constant for SH function
 */
float K(int l, int m) {
	float temp = ((2.0 * l + 1.0) * factorial(l - m)) / (4.0 * M_PI * factorial(
			l + m));
	return sqrt(temp);
}

/**
 * Return a point sample of a Spherical Harmonic basis function:
 * l is the band, range [0..N]
 * m in the range [-l..l]
 * theta in the range [0..Pi]
 * phi in the range [0..2*Pi]
 */
float SH(int l, int m, float theta, float phi) {


	return boost::math::spherical_harmonic_r(l, m, theta, phi);

//	const float sqrt2 = sqrt(2.0);
//	if (m == 0) {
//		return K(l, 0) * P(l, m, cos(theta));
//	} else if (m > 0) {
//		return sqrt2 * K(l, m) * cos(m * phi) * P(l, m, cos(theta));
//	} else {
//		return sqrt2 * K(l, -m) * sin(-m * phi) * P(l, -m, cos(theta));
//	}
}

/**
 * The number of spherical harmonics when using a l bands approximation.
 */
int nCoefficients(int l) {
	int m = 1;
	for (int i = 0; i < l; i++) {
		m += 2 * l + 1;
	}
	return m;
}

/**
 * Calculates the coefficients for the spherical harmonics using the given samples.
 */
void projectOnSH(Sample* samples, int nSamples, int nCoeffs, C3f result[]) {

	const float weight = 4.0 * M_PI;

	// clear the coeffs
	for (int n = 0; n < nCoeffs; n++) {
		result[n] = C3f(0,0,0);
	}

	// for each sample
	for (int i = 0; i < nSamples; ++i) {
		float theta = samples[i].spherDir.x;
		float phi = samples[i].spherDir.y;
		for (int n = 0; n < nCoeffs; n++) {
			result[n] += samples[i].radiance * samples[i].coeffSH[n];
		}
	}
	// divide the result by weight and number of samples
	float factor = weight / nSamples;
	for (int i = 0; i < nCoeffs; i++) {
		result[i] = result[i] * factor;
		Aqsis::log() << result[i] << " ";
	}

	Aqsis::log() << std::endl;

}

/**
 * Interpolate the radiance of sample using the submitted SH coeffs.
 */
void interpolateRadianceFromSH(int nCoeffs, Imath::C3f coeffs[], Sample sample) {
	for (int n = 0; n < nCoeffs; ++n) {
		sample.radiance += sample.coeffSH[n] * coeffs[n];
	}
}

V3f cartesianToSpherical(const V3f dir) {
	V3f tmp;
	tmp.z = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	tmp.x = acos(dir.z / tmp.z);
	tmp.y = atan(dir.y / dir.x);
	return tmp;
}

/**
 * Fill an N*N*2 array with uniformly distributed samples across the sphere
 * using jittered stratification.
 */
void setupSphericalSamples(Sample samples[], int sqrtnSamples, int nBands, CqRandom randomGenerator) {

	int i = 0; // array index
	int nCoeff = nCoefficients(nBands);
	double oneoverN = 1.0 / sqrtnSamples;
	for (int a = 0; a < sqrtnSamples; a++) {
		for (int b = 0; b < sqrtnSamples; b++) {
			// generate unbiased distribution of spherical coords
			double x = (a + randomGenerator.RandomFloat()) * oneoverN; // do not reuse results
			double y = (b + randomGenerator.RandomFloat()) * oneoverN; // each sample must be random
			double theta = 2.0 * acos(sqrt(1.0 - x));
			double phi = 2.0 * M_PI * y;
			samples[i].spherDir = V3f(theta, phi, 1.0);
			V3f vec(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
			samples[i].cartDir = vec;

			// precompute all SH coefficients for this sample
			for (int l = 0; l < nBands; ++l) {
				for (int m = -l; m <= l; ++m) {
					int index = l * (l + 1) + m;
					float coeff =  SH(l, m, theta, phi);
					samples[i].coeffSH[index] = C3f(coeff, coeff, coeff);
				}
			}
			++i;
		}
	}
}

/**
 * Calculate the spheric harmonic coefficents for the spheric direction in sample for nBands.
 */
void calcCoeffSH(Sample& sample, int nBands) {
	for (int l = 0; l < nBands; ++l) {
		for (int m = -l; m <= l; ++m) {
			int index = l * (l + 1) + m;
			float coeff = SH(l, m, sample.spherDir.x, sample.spherDir.y);
			sample.coeffSH[index] = C3f(coeff, coeff, coeff);
		}
	}
}

}
