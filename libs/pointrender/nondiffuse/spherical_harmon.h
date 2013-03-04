/*
 * spherical_harmon.h
 *
 *  Created on: Feb 27, 2013
 *      Author: karsten
 */

#ifndef SPHERICAL_HARMON_H_
#define SPHERICAL_HARMON_H_

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>
#include <OpenEXR/ImathMath.h>
#include <OpenEXR/ImathFun.h>

#include <aqsis/math/math.h>
#include <aqsis/math/color.h>
#include <aqsis/math/noise.h>
#include <aqsis/math/random.h>
#include <aqsis/math/vector3d.h>

namespace Aqsis {

struct Sample {
	Imath::V3f spherDir;
	Imath::V3f cartDir;
	Imath::C3f* coeffSH;
	Imath::C3f radiance;
};

/**
 * Return a point sample of a Spherical Harmonic basis function:
 * l is the band, range [0..N]
 * m in the range [-l..l]
 * theta in the range [0..Pi]
 * phi in the range [0..2*Pi]
 */
float SH(int l, int m, float theta, float phi);

/**
 * Calculates the coefficients for the spherical harmonics using the given samples.
 */
void projectOnSH(Sample* samples,	int nSamples, int nCoeffs, Imath::C3f coeffs[]);

/**
 * Interpolate the radiance of sample using the submitted SH coeffs.
 */
void interpolateRadianceFromSH(int nCoefficients, Imath::C3f coeffs[], Sample sample);

/**
 * Transforms dir into spherical coordinates.
 */
Imath::V3f cartesianToSpherical(const Imath::V3f dir);

/**
 * The number of spherical harmonics when using a l bands approximation.
 */
int nCoefficients(int l);

/**
 * Fill an N*N*2 array with uniformly distributed samples across the sphere
 * using jittered stratification.
 */
void setupSphericalSamples(Sample samples[], int sqrtnSamples, int nBands, CqRandom randomGenerator);

/**
 * Calculate the spheric harmonic coefficents for the spheric direction in sample for nBands.
 */
void calcCoeffSH(Sample& sample, int nBands);
}



#endif /* SPHERICAL_HARMON_H_ */
