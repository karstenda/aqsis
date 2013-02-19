/*
 * NonDiffusePointOctree.cpp
 *
 *  Created on: Jan 10, 2013
 *      Author: karsten
 */

#include "NonDiffusePoint.h"

#include "../../../thirdparty/pngpp/png.hpp"


namespace Aqsis {

using Imath::C3f;
using Imath::V3f;

NonDiffusePoint::NonDiffusePoint(const float* data, int faceRes):
		data(data),faceRes(faceRes) {
}

const float* NonDiffusePoint::getHemiFace(int f) {
	assert(f >= 0 && f < 6);
	return getHemi() + f*3*faceRes*faceRes;
}


C3f NonDiffusePoint::getRadiosityInDir(V3f dir) {

	MicroBuf::Face f = MicroBuf::faceIndex(dir);
	float u = 0;
	float v = 0;
	MicroBuf::faceCoords(f,dir,u,v);

    float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	int ui = Imath::clamp(int(u), 0, faceRes-1);
	int vi = Imath::clamp(int(v), 0, faceRes-1);

	const float* pix = getHemiFace(f)+(vi*faceRes + ui)*3;
	const C3f& radiosity = *(C3f*) (pix);
	return radiosity;
}

C3f NonDiffusePoint::getInterpolatedRadiosityInDir(V3f dir) {


	float wInt = 0.5;

	MicroBuf::Face f = MicroBuf::faceIndex(dir);
	float u = 0;
	float v = 0;
	MicroBuf::faceCoords(f,dir,u,v);

    float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	// Construct square boxes that will lie on the microbuffer, the area that
	// these boxes will span will be integrated to calculate the outgoing radiance.
	struct BoundData {
		MicroBuf::Face faceIndex;
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
		b.faceIndex = MicroBuf::neighbourU(f, 0);
		MicroBuf::faceCoords(b.faceIndex, dir, u, v);
		u = rasterScale * (u + 1.0f);
		v = rasterScale * (v + 1.0f);
		b.ubegin = u - wInt;
		b.uend = u + wInt;
		b.vbegin = v - wInt;
		b.vend = v + wInt;
	} else if (bd0.uend > faceRes) {
		// right neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = MicroBuf::neighbourU(f, 1);
		MicroBuf::faceCoords(b.faceIndex, dir, u, v);
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
		b.faceIndex = MicroBuf::neighbourV(f, 0);
		MicroBuf::faceCoords(b.faceIndex, dir, u, v);
		u = rasterScale * (u + 1.0f);
		v = rasterScale * (v + 1.0f);
		b.ubegin = u - wInt;
		b.uend = u + wInt;
		b.vbegin = v - wInt;
		b.vend = v + wInt;
	} else if (bd0.vend > faceRes) {
		// top neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = MicroBuf::neighbourV(f, 1);
		MicroBuf::faceCoords(b.faceIndex, dir, u, v);
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
				const C3f& radiosity = *(C3f*) (getHemiFace(bd.faceIndex)+(iv*faceRes + iu)*3);
				const float pixelSize = NonDiffusePoint::getPixelSize(faceRes,iu,iv);
				tot += radiosity*coverage*pixelSize;
				totCoverage += coverage*pixelSize;
			}
		}
	}
	if (totCoverage != 0) {
		tot = (1/totCoverage)*tot;
	}
	return tot;
}



C3f NonDiffusePoint::getInterpolatedRadiosityInDir2(V3f dir) {


	float wInt = 0.5;

	MicroBuf::Face f = MicroBuf::faceIndex(dir);
	float u = 0;
	float v = 0;
	MicroBuf::faceCoords(f,dir,u,v);

    float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	// Construct square boxes that will lie on the microbuffer, the area that
	// these boxes will span will be integrated to calculate the outgoing radiance.
	struct BoundData {
		MicroBuf::Face faceIndex;
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
		b.faceIndex = MicroBuf::neighbourU(f, 0);
		b.ubegin = faceRes + bd0.ubegin;
		b.uend = faceRes;
		b.vbegin = bd0.vbegin;
		b.vend = bd0.vend;
	} else if (bd0.uend > faceRes) {
		// right neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = MicroBuf::neighbourU(f, 1);
		b.ubegin = 0;
		b.uend = bd0.uend-faceRes;
		b.vbegin = bd0.vbegin;
		b.vend = bd0.vend;
	}
	if (bd0.vbegin < 0) {
		// bottom neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = MicroBuf::neighbourV(f, 0);
		b.ubegin = bd0.ubegin;
		b.uend = bd0.ubegin;
		b.vbegin = faceRes + bd0.vbegin;
		b.vend = faceRes;
	} else if (bd0.vend > faceRes) {
		// top neighbour
		BoundData& b = boundData[nfaces++];
		b.faceIndex = MicroBuf::neighbourV(f, 1);
		b.ubegin = bd0.ubegin;
		b.uend = bd0.uend;
		b.vbegin = 0;
		b.vend = bd0.vend-faceRes;
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
				const C3f& radiosity = *(C3f*) (getHemiFace(bd.faceIndex)+(iv*faceRes + iu)*3);
				const float pixelSize = NonDiffusePoint::getPixelSize(faceRes,iu,iv);
				tot += radiosity*coverage*pixelSize;
				totCoverage += coverage*pixelSize;
			}
		}
	}
	if (totCoverage != 0) {
		tot = (1/totCoverage)*tot;
	}
	return tot;
}


C3f NonDiffusePoint::getInterpolatedRadiosityInDir3(V3f dir) {

	MicroBuf::Face f = MicroBuf::faceIndex(dir);
	float u = 0;
	float v = 0;
	MicroBuf::faceCoords(f,dir,u,v);

    float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	struct Pixel {
		MicroBuf::Face face;
		float u;
		float v;
		float coverage;
	};

	Pixel covPixels[4];
	// Top Left Pixel;
	Pixel& tlP = covPixels[0];
	tlP.face = f;
	tlP.u = u - 0.5;
	tlP.v = v + 0.5;
	tlP.coverage = (ceil(tlP.u)-tlP.u)*(tlP.v-floor(tlP.v));
	// Top Right Pixel
	Pixel& trP = covPixels[1];
	trP.face = f;
	trP.u = u + 0.5;
	trP.v = v + 0.5;
	trP.coverage = (trP.u-floor(trP.u))*(trP.v-floor(trP.v));
	// Bottom Left Pixel
	Pixel& blP = covPixels[2];
	blP.face = f;
	blP.u = u - 0.5;
	blP.v = v + 0.5;
	blP.coverage = (ceil(blP.u)-blP.u)*(ceil(blP.v)-blP.v);
	// Bottom Right Pixel
	Pixel& brP = covPixels[3];
	brP.face = f;
	brP.u = u + 0.5;
	brP.v = v - 0.5;
	brP.coverage = (brP.u-floor(brP.u)*(ceil(brP.v)-brP.v));

	// Detect & handle overlap onto adjacent faces
	if (tlP.u < 0) {
		// left neighbour
		MicroBuf::Face lF = MicroBuf::neighbourU(f, 0);
		tlP.face = lF;
		blP.face = lF;
		tlP.u = faceRes + tlP.u;
		blP.u = faceRes + blP.u;

		// In case of corner, eliminate non existant faces by
		// setting coverage to 0.
		if (tlP.v > faceRes) {
			tlP.coverage = 0;
		} else if (blP.v > 0) {
			blP.coverage = 0;
		}
	} else if (trP.u > faceRes) {
		// right neighbour
		MicroBuf::Face rF = MicroBuf::neighbourU(f, 1);
		trP.face = rF;
		brP.face = rF;
		trP.u = trP.u-faceRes;
		brP.u = brP.u-faceRes;

		// In case of corner, eliminate non existant faces by
		// setting coverage to 0.
		if (trP.v > faceRes) {
			trP.coverage = 0;
		} else if (brP.v < 0) {
			brP.coverage = 0;
		}
	}

	if (blP.v < 0) {
		// bottom neighbour
		MicroBuf::Face bF = MicroBuf::neighbourV(f, 0);
		blP.face = bF;
		brP.face = bF;
		blP.v = faceRes + blP.v;
		brP.v = faceRes + brP.v;

		// Non existant faces already eliminated, nothing to do here.
	} else if (tlP.v > faceRes) {
		// top neighbour
		MicroBuf::Face tF = MicroBuf::neighbourV(f, 1);
		tlP.face = tF;
		trP.face = tF;
		tlP.v = tlP.v-faceRes;
		trP.v = trP.v-faceRes;

		// Non existant faces already eliminated, nothing to do here.
	}

	// Integrate over the pixels.
	C3f tot(0,0,0);
	float totCoverage = 0;
	for (int i = 0; i < 4; ++i) {
		Pixel pixel = covPixels[i];
		if (pixel.coverage != 0) {
			int iu = floor(pixel.u);
			int iv = floor(pixel.v);
			const C3f& radiosity = *(C3f*) (getHemiFace(pixel.face)+(iv*faceRes + iu)*3);
			tot += pixel.coverage*radiosity;
			totCoverage += pixel.coverage;
		}
	}
	if (totCoverage != 0) {
		tot = (1/totCoverage)*tot;
	}
	return tot;
}


int NonDiffusePoint::cachedRes = 10;
boost::scoped_array<float> NonDiffusePoint::cachedPixelSizes(NULL);

void calculatePixelSizeCache(int faceRes, boost::scoped_array<float>& cachedPixelSizes) {
	cachedPixelSizes.reset(new float[faceRes*faceRes*6]);
	for (int iv = 0; iv < faceRes; ++iv) {
		for (int iu = 0; iu < faceRes; ++iu) {
			float u = (0.5f + iu) / faceRes * 2.0f - 1.0f;
			float v = (0.5f + iv) / faceRes * 2.0f - 1.0f;
			cachedPixelSizes[iv * faceRes + iu] = 1.0f / V3f(u, v, 1).length2();
		}
	}
}



float NonDiffusePoint::getPixelSize(int faceRes, int u, int v) {
	if (cachedRes == faceRes && cachedPixelSizes.get() != NULL) {
		return cachedPixelSizes[faceRes * v + u];
	} else {
		cachedRes = faceRes;
		calculatePixelSizeCache(faceRes, cachedPixelSizes);
		return cachedPixelSizes[faceRes * v + u];
	}
}




int to8BitVal(float val) {
	return Imath::clamp(int(val * 255), 0, 255);
}

/// With the following enum indexes:
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
void NonDiffusePoint::writeMicroBufImage(std::string filename) {

	png::image<png::rgba_pixel> image(faceRes * 4, faceRes * 3);

	/**
	 * Set all the alpha vals on zero.
	 */
	for (size_t y = 0; y < image.get_height(); ++y)
	     for (size_t x = 0; x < image.get_width(); ++x)
	         image[y][x] = png::rgba_pixel(0, 0, 0, 0);


	/**
	 * Write face 0.
	 */
	int xFaceStart = 3 * faceRes;
	int yFaceStart = faceRes;
	const float* dataPt = getHemiFace(0);
	const float* pix = dataPt;
	for (size_t y = yFaceStart, v = 0; y < yFaceStart + faceRes; ++y, v++) {
		for (size_t x = xFaceStart, u = 0; x < xFaceStart + faceRes; ++x, u++) {
			pix = dataPt + (v * faceRes + u) * 3;
			image[y][x] = png::rgba_pixel(to8BitVal(pix[0]), to8BitVal(pix[1]),
					to8BitVal(pix[2]), 255);
		}
	}
	/**
	 * Write face 1.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = 2 * faceRes;
	dataPt = getHemiFace(1);
	for (size_t y = yFaceStart, v = 0; y < yFaceStart + faceRes; ++y, v++) {
		for (size_t x = xFaceStart, u = 0; x < xFaceStart + faceRes; ++x, u++) {
			pix = dataPt + (v * faceRes + u) * 3;
			image[y][x] = png::rgba_pixel(to8BitVal(pix[0]), to8BitVal(pix[1]),
					to8BitVal(pix[2]), 255);
		}
	}
	/**
	 * Write face 2.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = faceRes;
	dataPt = getHemiFace(2);
	for (size_t y = yFaceStart, v = 0; y < yFaceStart + faceRes; ++y, v++) {
		for (size_t x = xFaceStart, u = 0; x < xFaceStart + faceRes; ++x, u++) {
			pix = dataPt + (v * faceRes + u) * 3;
			image[y][x] = png::rgba_pixel(to8BitVal(pix[0]), to8BitVal(pix[1]),
					to8BitVal(pix[2]), 255);
		}
	}

	/**
	 * Write face 3.
	 */
	xFaceStart = faceRes;
	yFaceStart = faceRes;
	dataPt = getHemiFace(3);
	for (size_t y = yFaceStart, v = 0; y < yFaceStart + faceRes; ++y, v++) {
		for (size_t x = xFaceStart, u = 0; x < xFaceStart + faceRes; ++x, u++) {
			pix = dataPt + (v * faceRes + u) * 3;
			image[y][x] = png::rgba_pixel(to8BitVal(pix[0]), to8BitVal(pix[1]),
					to8BitVal(pix[2]), 255);
		}
	}

	/**
	 * Write face 4.
	 */
	xFaceStart = 2*faceRes;
	yFaceStart = 0;
	dataPt = getHemiFace(4);
	for (size_t y = yFaceStart, v = 0; y < yFaceStart + faceRes; ++y, v++) {
		for (size_t x = xFaceStart, u = 0; x < xFaceStart + faceRes; ++x, u++) {
			pix = dataPt + (v * faceRes + u) * 3;
			image[y][x] = png::rgba_pixel(to8BitVal(pix[0]), to8BitVal(pix[1]),
					to8BitVal(pix[2]), 255);
		}
	}
	/**
	 * Write face 5.
	 */
	xFaceStart = 0;
	yFaceStart = faceRes;
	dataPt = getHemiFace(5);
	for (size_t y = yFaceStart, v = 0; y < yFaceStart + faceRes; ++y, v++) {
		for (size_t x = xFaceStart, u = 0; x < xFaceStart + faceRes; ++x, u++) {
			pix = dataPt + (v * faceRes + u) * 3;
			image[y][x] = png::rgba_pixel(to8BitVal(pix[0]), to8BitVal(pix[1]),
					to8BitVal(pix[2]), 255);
		}
	}

	image.write(filename);
}



NonDiffusePoint::~NonDiffusePoint() {

}





}
