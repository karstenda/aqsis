/*
 * NonDiffuseSurphel.h
 *
 *  Created on: Nov 6, 2012
 *      Author: karsten
 */

#ifndef NONDIFFUSESURPHEL_HPP_
#define NONDIFFUSESURPHEL_HPP_

namespace Aqsis {

const float defPix[] = { 0, 0, 0, 0, 0 };

class NonDiffuseSurphel {

private:

	V3f position;
	V3f normal;
	float radius;
	int phong;
	RadiosityIntegrator integrator;

public:

	NonDiffuseSurphel(int faceres);
	NonDiffuseSurphel(V3f position, V3f normal, float radius, int phong,
			int faceRes, float* rawPixelData);
	NonDiffuseSurphel(const NonDiffuseSurphel& surphel);

	C3f getRadiosity(V3f direction);
	float* getRadiusPointer();
	int* getPhongPointer();
	float* getPixelPointer();
	V3f* getNormalPointer();
	V3f* getPositionPointer();

	virtual ~NonDiffuseSurphel();
};

}

#endif /* NONDIFFUSESURPHEL_HPP_ */
