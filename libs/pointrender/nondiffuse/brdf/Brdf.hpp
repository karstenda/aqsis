/*
 * Brdf.hpp
 *
 *  Created on: Jan 28, 2013
 *      Author: karsten
 */

#ifndef BRDF_HPP_
#define BRDF_HPP_

#include <aqsis/aqsis.h>
#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathMatrix.h>

namespace Aqsis {

class Brdf {

public:

	/**
	 *	This function returns a random vector of unit length. The directions of the vectors resulting
	 *	from this method are distributed over the hemisphere according to probability density function
	 *	of the outgoing radiance of the BRDF.
	 *	- imcoming: The direction of the incoming radiance.
	 *	- normal: The normal of the surface.
	 *	- direction: The random direction of the outgoing radiance.
	 */
	virtual void getRandomDirection(const Imath::V3f incoming,
			const Imath::V3f normal, Imath::V3f direction) = 0;

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
			const Imath::V3f normal, int n, Imath::V3f* directions) = 0;

	virtual std::string toString() const = 0;

	virtual ~Brdf() {
	};

};

}
#endif /* BRDF_HPP_ */
