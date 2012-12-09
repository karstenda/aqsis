/*
 * NonDiffuseSurphel.h
 *
 *  Created on: Nov 6, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSESURPHEL_HPP_
#define NONDIFFUSESURPHEL_HPP_


#include	"../RadiosityIntegrator.h"


namespace Aqsis {

const float defPix[] = { 0, 0, 0, 0, 0 };

class NonDiffuseSurphel {

private:

	Imath::V3f position;
	Imath::V3f normal;
	float radius;
	int phong;
	RadiosityIntegrator integrator;

public:

	NonDiffuseSurphel(int faceres);
	NonDiffuseSurphel(Imath::V3f position, Imath::V3f normal, float radius, int phong,
			int faceRes, float* rawPixelData);
	NonDiffuseSurphel(const NonDiffuseSurphel& surphel);

	Imath::C3f getRadiosity(Imath::V3f direction);
	float* getRadiusPointer();
	int* getPhongPointer();
	float* getPixelPointer();
	Imath::V3f* getNormalPointer();
	Imath::V3f* getPositionPointer();

	virtual ~NonDiffuseSurphel();
};

}

#endif /* NONDIFFUSESURPHEL_HPP_ */
