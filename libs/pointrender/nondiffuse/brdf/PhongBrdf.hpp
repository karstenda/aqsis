/*
 * PhongBrdf.h
 *
 *  Created on: Jan 28, 2013
 *      Author: karsten
 */

#ifndef PHONGBRDF_HPP_
#define PHONGBRDF_HPP_

#include "Brdf.hpp"

#include	<aqsis/math/math.h>
#include	<aqsis/math/color.h>
#include	<aqsis/math/noise.h>
#include	<aqsis/math/random.h>
#include	<aqsis/math/vector3d.h>

namespace Aqsis {

class PhongBrdf: public Brdf {

private:
	float phongExponent;
	CqRandom randomGenerator;

public:
	PhongBrdf(float phongExponent, CqRandom randomGenerator);

	/**
	 *	This function returns a random vector of unit length. The directions of the vectors resulting
	 *	from this method are distributed over the hemisphere according to probability density function
	 *	of the outgoing radiance of the BRDF.
	 *	- imcoming: The direction of the incoming radiance.
	 *	- normal: The normal of the surface.
	 *	- direction: The random direction of the outgoing radiance.
	 */
	virtual void getRandomDirection(const Imath::V3f incoming,
			const Imath::V3f normal, Imath::V3f direction);

	/**
	 * This function generates 'n' random directions of unit length. The directions of the vectors resulting
	 *	from this method are distributed over the hemisphere according to probability density function
	 *	of the outgoing radiance of the BRDF.
	 *	- imcoming: The direction of the incoming radiance.
	 *	- normal: The normal of the surface.
	 *	- n: The number of random outgoing directions to generate.
	 *	- directions: The pointer to the array where the random outgoing directions will be saved.
	 */
	virtual void getRandomDirections(const Imath::V3f incoming,
			const Imath::V3f normal, int n, Imath::V3f* directions);

	virtual std::string toString() const;


	virtual ~PhongBrdf();

private:

	void randVonMisesFisher3(Imath::V3f mu, float kappa, int n, Imath::V3f* directions);

};

}

#endif /* PHONGBRDF_HPP_ */
