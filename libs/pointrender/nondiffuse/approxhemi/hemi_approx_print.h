/*
 * hemi_approx_print.h
 *
 *  Created on: Mar 27, 2013
 *      Author: karsten
 */

#ifndef HEMI_APPROX_PRINT_H_
#define HEMI_APPROX_PRINT_H_


#include "HemiApprox.h"
#include "CubeMapApprox.h"
#include "../Hemisphere.h"
#include "../../MicroBuf.h"
#include "../../../../thirdparty/pngpp/png.hpp"


namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

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

void writeHemiApproxImage(std::string filename, int faceRes, HemiApprox* hemi) {

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
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float u = ui / (faceRes * 0.5) - 1;
			float v = vi / (faceRes * 0.5) - 1;
			V3f dir = CubeMapApprox::direction(0, u, v);
			C3f col = hemi->getRadiosityInDir(dir);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}
	/**
	 * Write face 1.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = 2 * faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float u = ui / (faceRes * 0.5) - 1;
			float v = vi / (faceRes * 0.5) - 1;
			V3f dir = CubeMapApprox::direction(1, u, v);
			C3f col = hemi->getRadiosityInDir(dir);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 2.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float u = ui / (faceRes * 0.5) - 1;
			float v = vi / (faceRes * 0.5) - 1;
			V3f dir = CubeMapApprox::direction(2, u, v);
			C3f col = hemi->getRadiosityInDir(dir);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 3.
	 */
	xFaceStart = faceRes;
	yFaceStart = faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float u = ui / (faceRes * 0.5) - 1;
			float v = vi / (faceRes * 0.5) - 1;
			V3f dir = CubeMapApprox::direction(3, u, v);
			C3f col = hemi->getRadiosityInDir(dir);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 4.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = 0;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float u = ui / (faceRes * 0.5) - 1;
			float v = vi / (faceRes * 0.5) - 1;
			V3f dir = CubeMapApprox::direction(4, u, v);
			C3f col = hemi->getRadiosityInDir(dir);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 5.
	 */
	xFaceStart = 0;
	yFaceStart = faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float u = ui / (faceRes * 0.5) - 1;
			float v = vi / (faceRes * 0.5) - 1;
			V3f dir = CubeMapApprox::direction(5, u, v);
			C3f col = hemi->getRadiosityInDir(dir);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	image.write(filename);
}

void writeMicroBufImage(std::string filename, MicroBuf& microbuf) {

	int faceRes = microbuf.getFaceResolution();
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
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float* pix = microbuf.face(0)+(vi*faceRes + ui)*microbuf.nchans();
			C3f& col = *(C3f*) (pix + 2);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}
	/**
	 * Write face 1.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = 2 * faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float* pix = microbuf.face(1)+(vi*faceRes + ui)*microbuf.nchans();
						C3f& col = *(C3f*) (pix + 2);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 2.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float* pix = microbuf.face(2)+(vi*faceRes + ui)*microbuf.nchans();
			C3f& col = *(C3f*) (pix + 2);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 3.
	 */
	xFaceStart = faceRes;
	yFaceStart = faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float* pix = microbuf.face(3)+(vi*faceRes + ui)*microbuf.nchans();
			C3f& col = *(C3f*) (pix + 2);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 4.
	 */
	xFaceStart = 2 * faceRes;
	yFaceStart = 0;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float* pix = microbuf.face(4)+(vi*faceRes + ui)*microbuf.nchans();
			C3f& col = *(C3f*) (pix + 2);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	/**
	 * Write face 5.
	 */
	xFaceStart = 0;
	yFaceStart = faceRes;
	for (size_t y = yFaceStart, vi = 0; y < yFaceStart + faceRes; ++y, vi++) {
		for (size_t x = xFaceStart, ui = 0; x < xFaceStart + faceRes; ++x, ui++) {
			float* pix = microbuf.face(5)+(vi*faceRes + ui)*microbuf.nchans();
			C3f& col = *(C3f*) (pix + 2);
			image[y][x] = png::rgba_pixel(to8BitVal(col.x), to8BitVal(col.y),
					to8BitVal(col.z), 255);
		}
	}

	image.write(filename);
}


}

#endif
