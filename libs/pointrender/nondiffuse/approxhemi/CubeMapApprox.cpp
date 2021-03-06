/*
 * MicroBufApprox.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: karsten
 */

#include "CubeMapApprox.h"

#include <string.h>
#include <aqsis/util/logging.h>


namespace Aqsis {

using Imath::V3f;
using Imath::C3f;
using std::vector;


CubeMapApprox::CubeMapApprox(int faceRes):
		data(faceRes*faceRes*3*6,0), faceRes(faceRes), size(faceRes*faceRes*3*6) {

}

CubeMapApprox::CubeMapApprox(const float* data1, int length):
		data(length-1,0), faceRes(0), size(length-1) {

	for (int i=0; i < size; i++) {
		data[i] = data1[i+1];
	}

	// Determine the face Resolution
	float res = sqrt(size/(6*3));
	int intres1 = floor(res);
	int intres2 = ceil(res);
	if (intres1 != intres2) {
		Aqsis::log() << error << "Invalid hemisphere resolution in loaded pointfile: "<< res << std::endl;
	}
	faceRes = intres1;
}


float* CubeMapApprox::getFace(int f) {
	return &data[f*3*faceRes*faceRes];
}



///
///              +---+
///   ^          | 1 |
///   |  +---+---+---+---+
///  v|  | 5 | 3 | 2 | 0 |
///   |  +---+---+---+---+
///   |          | 4 |
///   |          +---+
///   |     u
///   +-------->
V3f CubeMapApprox::direction(int faceIdx, float u, float v) {
	switch (faceIdx) {
	case 0:
		return V3f(1, v, -u).normalized();
	case 1:
		return V3f(u, 1, -v).normalized();
	case 2:
		return V3f(u, v, 1).normalized();
	case 3:
		return V3f(-1, v, u).normalized();
	case 4:
		return V3f(u, -1, v).normalized();
	case 5:
		return V3f(-u, v, -1).normalized();
	default:
		assert(0 && "unknown face");
		return V3f();
	}
}

int CubeMapApprox::faceIndex(V3f p) {
	V3f absp = V3f(fabs(p.x), fabs(p.y), fabs(p.z));
	if (absp.x >= absp.y && absp.x >= absp.z)
		return (p.x > 0) ? 0 : 3;
	else if (absp.y >= absp.x && absp.y >= absp.z)
		return (p.y > 0) ? 1 : 4;
	else {
		assert(absp.z >= absp.x && absp.z >= absp.y);
		return (p.z > 0) ? 2 : 5;
	}
}

V3f CubeMapApprox::canonicalFaceCoords(int faceIdx, V3f p) {
	switch (faceIdx) {
	case 0:
		return V3f(-p.z, p.y, p.x);
	case 3:
		return V3f(-p.z, -p.y, p.x);
	case 1:
		return V3f(p.x, -p.z, p.y);
	case 4:
		return V3f(-p.x, -p.z, p.y);
	case 2:
		return V3f(p.x, p.y, p.z);
	case 5:
		return V3f(p.x, -p.y, p.z);
	default:
		assert(0 && "invalid face");
		return V3f();
	}
}

void CubeMapApprox::faceCoords(int faceIdx, V3f p, float& u, float& v) {
	p = canonicalFaceCoords(faceIdx, p);
	assert(p.z != 0);
	float zinv = 1.0 / p.z;
	u = p.x * zinv;
	v = p.y * zinv;
}


int CubeMapApprox::neighbourU(int faceIdx, int side) {
	static int neighbourArray[6][2] = { { 2, 5 }, { 3, 0 }, { 3, 0 },
			{ 5, 2 }, { 3, 0 }, { 0, 3 } };
	return neighbourArray[faceIdx][side];
}

int CubeMapApprox::neighbourV(int faceIdx, int side) {
	static int neighbourArray[6][2] = { { 4, 1 }, { 2, 5 }, { 4, 1 },
			{ 4, 1 }, { 5, 2 }, { 4, 1 } };
	return neighbourArray[faceIdx][side];
}

int CubeMapApprox::calculateFloatArraySize(int faceRes) {
	return faceRes*faceRes*18+1;
}


void CubeMapApprox::approximate(const Hemisphere& hemi) {

	for (int f = 0; f < 6; ++f) {
		const float* pixel = getFace(f);
		for (int iv = 0; iv < faceRes; ++iv) {
			for (int iu = 0; iu < faceRes; ++iu, pixel+=3) {

				float u = (0.5f + iu) / faceRes * 2.0f - 1.0f;
				float v = (0.5f + iv) / faceRes * 2.0f - 1.0f;
				V3f dir = direction(f, u, v);

				C3f col = hemi.radiosityInDir(dir);

				C3f* rad = (C3f*) (pixel);
				rad->x = col.x;
				rad->y = col.y;
				rad->z = col.z;

			}
		}
	}
}

C3f CubeMapApprox::getRadiosityInDir2(V3f dir) {

	int f = faceIndex(dir);
	float u = 0;
	float v = 0;
	faceCoords(f, dir, u, v);

	float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	int iv = (int) v;
	int iu = (int) u;

	const C3f& radiosity = *(C3f*) (getFace(f)+(iv + iu*faceRes)*3);
	return radiosity;
}

C3f CubeMapApprox::getRadiosityInDir(V3f dir, float distRatio) {
	return getRadiosityInDir(dir);
}

C3f CubeMapApprox::getRadiosityInDir(V3f dir) {


	float wInt = 0.5;

	int f = faceIndex(dir);
	float u = 0;
	float v = 0;
	faceCoords(f,dir,u,v);

    float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	// Construct square boxes that will lie on the microbuffer, the area that
	// these boxes will span will be integrated to calculate the outgoing radiance.
	struct BoundData {
		int faceIndex;
		float ubegin, uend;
		float vbegin, vend;
	};

	// The domain to integrate can cross up to three faces.
	int nfaces = 1;
	BoundData boundData[3];
	BoundData& bd0 = boundData[0];
	bd0.faceIndex = f;
	bd0.ubegin = u - wInt;   bd0.uend = u + wInt;
	bd0.vbegin = v - wInt;   bd0.vend = v + wInt;

	// Detect & handle overlap onto adjacent faces
	if (bd0.ubegin < 0) {
		// left neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = neighbourU(f, 0);
		faceCoords(b.faceIndex, dir, u, v);
		u = rasterScale * (u + 1.0f);
		v = rasterScale * (v + 1.0f);
		b.ubegin = u - wInt;
		b.uend = u + wInt;
		b.vbegin = v - wInt;
		b.vend = v + wInt;
	} else if (bd0.uend > faceRes) {
		// right neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = neighbourU(f, 1);
		faceCoords(b.faceIndex, dir, u, v);
		u = rasterScale * (u + 1.0f);
		v = rasterScale * (v + 1.0f);
		b.ubegin = u - wInt;
		b.uend = u + wInt;
		b.vbegin = v - wInt;
		b.vend = v + wInt;
	}
	if (bd0.vbegin < 0) {
		// bottom neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = neighbourV(f, 0);
		faceCoords(b.faceIndex, dir, u, v);
		u = rasterScale * (u + 1.0f);
		v = rasterScale * (v + 1.0f);
		b.ubegin = u - wInt;
		b.uend = u + wInt;
		b.vbegin = v - wInt;
		b.vend = v + wInt;
	} else if (bd0.vend > faceRes) {
		// top neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = neighbourV(f, 1);
		faceCoords(b.faceIndex, dir, u, v);
		u = rasterScale * (u + 1.0f);
		v = rasterScale * (v + 1.0f);
		b.ubegin = u - wInt;
		b.uend = u + wInt;
		b.vbegin = v - wInt;
		b.vend = v + wInt;
	}
	C3f tot(0,0,0);
	float totCoverage = 0;
	for (int iface = 0; iface < nfaces; ++iface) {
		BoundData& bd = boundData[iface];
		// Range of pixels which the square touches (note, exclusive end)
		int ubeginRas = Imath::clamp(int(bd.ubegin), 0, faceRes);
		int uendRas = Imath::clamp(int(bd.uend) + 1, 0, faceRes);
		int vbeginRas = Imath::clamp(int(bd.vbegin), 0, faceRes);
		int vendRas = Imath::clamp(int(bd.vend) + 1, 0, faceRes);
		for (int iv = vbeginRas; iv < vendRas; ++iv) {
			for (int iu = ubeginRas; iu < uendRas; ++iu) {
				// Calculate the fraction coverage of the square over the current
				// pixel for antialiasing.  This estimate is what you'd get if you
				// filtered the square representing the surfel with a 1x1 box filter.
				float urange = std::min<float>(iu + 1, bd.uend) - std::max<
						float>(iu, bd.ubegin);
				float vrange = std::min<float>(iv + 1, bd.vend) - std::max<
						float>(iv, bd.vbegin);
				float coverage = urange * vrange;

				const C3f& radiosity = *(C3f*) (getFace(bd.faceIndex)+(iv*faceRes + iu)*3);
				tot += radiosity*coverage;
				totCoverage += coverage;
			}
		}
	}
	if (totCoverage != 0) {
		tot = (1/totCoverage)*tot;
	}
	return tot;
}

int CubeMapApprox::getFloatArraySize() {
	return size + 1;
}

int CubeMapApprox::writeToFloatArray(float* data1) {
	data1[0] = (float) HemiApprox::CubeMap;
//	memcpy(&data1[1],data,size);

	for (int i=0; i < size; i++) {
		data1[1+i] = data[i];
	}

	return size + 1;
}

HemiApprox::Type CubeMapApprox::getType() {
	return HemiApprox::CubeMap;
}


void CubeMapApprox::add(const HemiApprox* other) {
	const CubeMapApprox* otherc = dynamic_cast<const CubeMapApprox*>(other);
	if (otherc) {
		*this += *otherc;
	} else {
		Aqsis::log() << error << "Can not add two different types of HemiApprox together." << std::endl;
	}
}

CubeMapApprox& CubeMapApprox::operator+= ( const CubeMapApprox& other) {
	assert(this->faceRes == other.faceRes);
	for (int i=0; i < size; i++) {
		this->data[i] += other.data[i];
	}
	return *this;
}

HemiApprox* CubeMapApprox::getDarkApprox() {
	return new CubeMapApprox(faceRes);
}

}
