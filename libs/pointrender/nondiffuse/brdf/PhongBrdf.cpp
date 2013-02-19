/*
 * PhongBrdf.cpp
 *
 *  Created on: Jan 28, 2013
 *      Author: karsten
 */

#include "PhongBrdf.hpp"

namespace Aqsis {

using Imath::V3f;
using Imath::M33f;


PhongBrdf::PhongBrdf(float phongExponent, CqRandom randomGenerator):
		phongExponent(phongExponent), randomGenerator(randomGenerator) {}

PhongBrdf::~PhongBrdf() {}


std::string PhongBrdf::toString() const {
	return "Phong";
}

inline float dot(V3f a, V3f b) {
	return a ^ b;
}

void PhongBrdf::getRandomDirection(const V3f incoming,
			const V3f normal, V3f direction) {

	getRandomDirections(incoming,normal,1,&direction);
}


void PhongBrdf::getRandomDirections(const V3f incoming,
			const V3f normal, int n, V3f* directions) {

	V3f R = -incoming - 2 * (dot(-incoming, normal)) * normal;
	R = R.normalize();

	randVonMisesFisher3(R, phongExponent, n, directions);
}


void PhongBrdf::randVonMisesFisher3(V3f mu, float kappa, int n, V3f* directions) {


	V3f normal(0,0,1);
	V3f u = mu.cross(normal);
	float cost = dot(mu,normal);
	float sint = u.length();
	u = u.normalize();

	M33f rot(cost + u.x * u.x * (1 - cost),
			u.x * u.y * (1 - cost) - u.z * sint,
			u.x * u.z * (1 - cost) + u.y * sint,
			u.y * u.x * (1 - cost) + u.z * sint,
			cost + u.y * u.y * (1 - cost),
			u.y * u.z * (1 - cost) - u.x * sint,
			u.z * u.x * (1 - cost) - u.y * sint,
			u.z	* u.y * (1 - cost) + u.x * sint,
			cost + u.z * u.z * (1 - cost));

	float c = 2/kappa*(sinh(kappa)); // normalizing constant

	float y, w, v;
	for (int i=0; i < n; i++) {
		y = randomGenerator.RandomFloat();
		w = 1/kappa * log( exp(-kappa) + kappa * c * y );
		v = 2*M_PI*randomGenerator.RandomFloat();

		directions[i].x = sqrt(1-w*w)*cos(v);
		directions[i].y = sqrt(1-w*w)*sin(v);
		directions[i].z = w;

		directions[i] = directions[i]*rot;
	}

}



}
