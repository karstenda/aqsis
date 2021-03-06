/*
 * HemiApprox.h
 *
 *  Created on: Mar 27, 2013
 *      Author: karsten
 */

#ifndef CUBEMAPAPPROX_H_
#define CUBEMAPAPPROX_H_

#include "HemiApprox.h"

#include "../Hemisphere.h"
#include <vector>

namespace Aqsis {

class CubeMapApprox: public HemiApprox {

private:
	std::vector<float> data;
	int faceRes;
	int size;

public:

	CubeMapApprox(int faceRes);

	CubeMapApprox(const float* data, int length);

	void approximate(const Hemisphere& hemi);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction, float distRatio);

	Imath::C3f getRadiosityInDir2(const Imath::V3f direction);


	int getFloatArraySize();

	int writeToFloatArray(float* data);

	float* getFace(int i);

	HemiApprox::Type getType();

	CubeMapApprox& operator+= ( const CubeMapApprox& other);

	void add(const HemiApprox* other);

	HemiApprox* getDarkApprox();




	static int neighbourU(int faceIdx, int side);

	static int neighbourV(int faceIdx, int side);

	static Imath::V3f direction(int faceIdx, float u, float v);

	static int faceIndex(Imath::V3f p);

	static Imath::V3f canonicalFaceCoords(int faceIdx, Imath::V3f p);

	static void faceCoords(int faceIdx, Imath::V3f p, float& u, float& v);

	static float getPixelSize(int res, int u, int v);

	static int calculateFloatArraySize(int faceRes);


	~CubeMapApprox() {	}
};

}


#endif /* MICROBUFAPPROX_H_ */
