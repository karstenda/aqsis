// Copyright (C) 2001, Paul C. Gregory and the other authors and contributors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the software's owners nor the names of its
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// (This is the New BSD license)


#include <cfloat>
#include <cmath>
#include <cstring>

#include <boost/scoped_array.hpp>

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathMath.h>
#include <OpenEXR/ImathFun.h>

#include "MicroBuf.h"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

const V3f MicroBuf::Normal = Imath::V3f(0,0,-1);


/// faceRes gives face resolution.  Faces are square.  nchans gives
/// the number of channels in each pixel, and defaultPix gives the
/// values of the channels which will be used when reset() is called.
MicroBuf::MicroBuf(int faceRes, int nchans, const float* defaultPix) :
	m_res(faceRes), m_nchans(nchans), m_faceSize(nchans * faceRes * faceRes),
			m_pixels() {
	m_pixels.reset(new float[m_faceSize * Face_end]);
	m_defaultPixels.reset(new float[m_faceSize * Face_end]);
	m_directions.reset(new V3f[Face_end * faceRes * faceRes]);
	m_pixelSizes.reset(new float[m_faceSize]);
	// Cache direction vectors
	for (int face = 0; face < Face_end; ++face) {
		for (int iv = 0; iv < m_res; ++iv)
			for (int iu = 0; iu < m_res; ++iu) {
				// directions of pixels go through pixel centers
				float u = (0.5f + iu) / faceRes * 2.0f - 1.0f;
				float v = (0.5f + iv) / faceRes * 2.0f - 1.0f;
				m_directions[(face * m_res + iv) * m_res + iu] = direction(
						face, u, v);
			}
	}

	// Calculate the solid angle for each pixel.
	for (int iv = 0; iv < m_res; ++iv) {
		for (int iu = 0; iu < m_res; ++iu) {
			float u = (0.5f + iu) / faceRes * 2.0f - 1.0f;
			float v = (0.5f + iv) / faceRes * 2.0f - 1.0f;
			float d2 = V3f(u, v, 1).length2(); // squared length to the pixel center
			float cosFac = dotFaceNormal(Face_zp, rayDirection(Face_zp, iu, iv)); // cos between facenormal and dir
			float area = (2.f/faceRes)*(2.f/faceRes); // area of a pixel
			m_pixelSizes[iv * m_res + iu] =  ((area*cosFac)/d2);
			// m_pixelSizes[iv * m_res + iu] =  1.f/d2; // legacy val
		}
	}


	float* pix = m_defaultPixels.get();
	for (int i = 0, iend = size(); i < iend; ++i, pix += m_nchans)
		for (int c = 0; c < m_nchans; ++c)
			pix[c] = defaultPix[c];
}

MicroBuf::MicroBuf(MicroBuf& microbuf) :
			m_res(microbuf.m_res),
			m_nchans(microbuf.m_nchans),
			m_faceSize(microbuf.m_faceSize),
			m_pixels(microbuf.getPixelPointer()) {

}

void MicroBuf::reset(float* pixels) {

	m_pixels.reset(pixels);
//	memcpy(m_pixels.get(), pixels, sizeof(float) * size() * m_nchans);
}

/// Reset buffer to default (non-rendered) state.
void MicroBuf::reset() {
	memcpy(m_pixels.get(), m_defaultPixels.get(), sizeof(float) * size()
			* m_nchans);
}

/// Get raw data store for face
float* MicroBuf::face(int which) {
	assert(which >= Face_begin && which < Face_end);
	return &m_pixels[0] + which * m_faceSize;
}
const float* MicroBuf::face(int which) const {
	assert(which >= Face_begin && which < Face_end);
	return &m_pixels[0] + which * m_faceSize;
}

/// Get index of face which direction p sits inside.
MicroBuf::Face MicroBuf::faceIndex(V3f p) {
	V3f absp = V3f(fabs(p.x), fabs(p.y), fabs(p.z));
	if (absp.x >= absp.y && absp.x >= absp.z)
		return (p.x > 0) ? Face_xp : Face_xn;
	else if (absp.y >= absp.x && absp.y >= absp.z)
		return (p.y > 0) ? Face_yp : Face_yn;
	else {
		assert(absp.z >= absp.x && absp.z >= absp.y);
		return (p.z > 0) ? Face_zp : Face_zn;
	}
}

/// Get a neighbouring face in u direction
///
/// \param faceIdx - current face index
/// \param side - which side to look (0 == left, 1 == right)
///
// +---+---+---+  +---+---+---+  +---+---+---+
// |+z |+x |-z |  |-x |+y |+x |  |-x |+z |+x |
// +---+---+---+  +---+---+---+  +---+---+---+
//
// +---+---+---+  +---+---+---+  +---+---+---+
// |-z |-x |+z |  |-x |-y |+x |  |+x |-z |-x |
// +---+---+---+  +---+---+---+  +---+---+---+
//
MicroBuf::Face MicroBuf::neighbourU(int faceIdx, int side) {
	static Face neighbourArray[6][2] = { { Face_zp, Face_zn }, { Face_xn,
			Face_xp }, { Face_xn, Face_xp }, { Face_zn, Face_zp }, { Face_xn,
			Face_xp }, { Face_xp, Face_xn } };
	return neighbourArray[faceIdx][side];
}

/// Get a neighbouring face in v direction
///
/// \param faceIdx - current face index
/// \param side - which side to look (0 == bottom, 1 == top)
///
// +---+   +---+   +---+   +---+   +---+   +---+
// |+y |   |-z |   |+y |   |+y |   |+z |   |+y |
// +---+   +---+   +---+   +---+   +---+   +---+
// |+x |   |+y |   |+z |   |-x |   |-y |   |-z |
// +---+   +---+   +---+   +---+   +---+   +---+
// |-y |   |+z |   |-y |   |-y |   |-z |   |-y |
// +---+   +---+   +---+   +---+   +---+   +---+
MicroBuf::Face MicroBuf::neighbourV(int faceIdx, int side) {
	static Face neighbourArray[6][2] = { { MicroBuf::Face_yn, Face_yp }, {
			Face_zp, Face_zn }, { Face_yn, Face_yp }, { Face_yn, Face_yp }, {
			Face_zn, Face_zp }, { Face_yn, Face_yp } };
	return neighbourArray[faceIdx][side];
}

/// Get coordinates on face
///
/// The coordinates are in the range -1 <= u,v <= 1, if faceIdx is
/// obtained using the faceIndex function.  Coordinates outside this
/// range are legal, as long as p has nonzero component in the
/// direction of the normal of the face.
///
/// \param faceIdx - index of current face
/// \param p - position (may lie outside cone of current face)
void MicroBuf::faceCoords(int faceIdx, V3f p, float& u, float& v) {
	p = canonicalFaceCoords(faceIdx, p);
	assert(p.z != 0);
	float zinv = 1.0 / p.z;
	u = p.x * zinv;
	v = p.y * zinv;
}

/// Compute dot product of vec with face normal on given face
float MicroBuf::dotFaceNormal(int faceIdx, V3f vec) {
	assert(faceIdx < Face_end && faceIdx >= Face_begin);
	return (faceIdx < 3) ? vec[faceIdx] : -vec[faceIdx - 3];
}

/// Compute face normal
V3f MicroBuf::faceNormal(int faceIdx) {
	static V3f normals[6] = { V3f(1, 0, 0), V3f(0, 1, 0), V3f(0, 0, 1), V3f(-1,
			0, 0), V3f(0, -1, 0), V3f(0, 0, -1) };
	return normals[faceIdx];
}

/// Get direction vector for pixel on given face.
V3f MicroBuf::rayDirection(int faceIdx, int u, int v) const {
	return m_directions[(faceIdx * m_res + v) * m_res + u];
}

/// Return relative size of pixel.
///
/// Compared to a pixel in the middle of the cube face, pixels in the
/// corners of the cube have a smaller angular size.  We must take
/// this into account when integrating the radiosity.
///
/// \param u,v - face coordinates
float MicroBuf::pixelSize(int u, int v) const {
	return m_pixelSizes[m_res * v + u];
}

/// Reorder vector components into "canonical face coordinates".
///
/// The canonical coordinates correspond to the coordinates on the +z
/// face.  If we let the returned vector be q then (q.x, q.y)
/// correspond to the face (u, v) coordinates, and q.z corresponds to
/// the signed depth out from the face.
V3f MicroBuf::canonicalFaceCoords(int faceIdx, V3f p) {
	switch (faceIdx) {
	case Face_xp:
		return V3f(-p.z, p.y, p.x);
	case Face_xn:
		return V3f(-p.z, -p.y, p.x);
	case Face_yp:
		return V3f(p.x, -p.z, p.y);
	case Face_yn:
		return V3f(-p.x, -p.z, p.y);
	case Face_zp:
		return V3f(p.x, p.y, p.z);
	case Face_zn:
		return V3f(p.x, -p.y, p.z);
	default:
		assert(0 && "invalid face");
		return V3f();
	}
}

C3f MicroBuf::getRadiosityInDir(const V3f direction) const {
	Face f = faceIndex(direction);
	float u = 0;
	float v = 0;
	faceCoords(f,direction,u,v);

	int faceRes = getFaceResolution();
    float rasterScale = 0.5f*faceRes;
	u = rasterScale*(u + 1.0f);
	v = rasterScale*(v + 1.0f);

	float ul = u - 0.5;
	float ur = u + 0.5;
	float vu = v + 0.5;
	float vd = v - 0.5;


	int ui = Imath::clamp(int(u), 0, faceRes);
	int vi = Imath::clamp(int(v), 0, faceRes);

	const float* pix = face(f)+(vi*faceRes + ui)*nchans();
	const C3f& radiosity = *(C3f*) (pix + 2);
	return radiosity;
}

float* MicroBuf::getPixelPointer() {
	return &m_pixels[0];
}

int MicroBuf::getFaceResolution() const {
	return m_res;
}

int MicroBuf::getNChans() const {
	return m_nchans;
}

/// Face side resolution
int MicroBuf::res() const {
	return m_res;
}

/// Number of channels per pixel
int MicroBuf::nchans() const {
	return m_nchans;
}

/// Total size of all faces in number of texels
int MicroBuf::size() const {
	return Face_end * m_res * m_res;
}

/// Get direction vector for position on a given face.
///
/// Roughly speaking, this is the opposite of the faceCoords function
V3f MicroBuf::direction(int faceIdx, float u, float v) {
	switch (faceIdx) {
	case Face_xp:
		return V3f(1, v, -u).normalized();
	case Face_yp:
		return V3f(u, 1, -v).normalized();
	case Face_zp:
		return V3f(u, v, 1).normalized();
	case Face_xn:
		return V3f(-1, v, u).normalized();
	case Face_yn:
		return V3f(u, -1, v).normalized();
	case Face_zn:
		return V3f(-u, v, -1).normalized();
	default:
		assert(0 && "unknown face");
		return V3f();
	}
}

C3f MicroBuf::addRadiosityInDir(const V3f dir, const C3f incomingRad) {

	int faceRes = m_res;
	float wInt = 0.5;
	Face f = faceIndex(dir);
	float u = 0;
	float v = 0;
	faceCoords(f, dir, u, v);

	float rasterScale = 0.5f * faceRes;
	u = rasterScale * (u + 1.0f);
	v = rasterScale * (v + 1.0f);

	// Construct square boxes that will lie on the microbuffer, the area that
	// these boxes will span will be integrated to calculate the outgoing radiance.
	struct BoundData {
		Face faceIndex;
		float ubegin, uend;
		float vbegin, vend;
	};

	// The domain to integrate can cross up to three faces.
	int nfaces = 1;
	BoundData boundData[3];
	BoundData& bd0 = boundData[0];
	bd0.faceIndex = f;
	bd0.ubegin = u - wInt;
	bd0.uend = u + wInt;
	bd0.vbegin = v - wInt;
	bd0.vend = v + wInt;

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
		b.uend = bd0.uend - faceRes;
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
		b.vend = bd0.vend - faceRes;
	}
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

				C3f& radiosity = *(C3f*) (face(bd.faceIndex) + (iv*faceRes + iu) * m_nchans + 2);
				radiosity += coverage * incomingRad;
			}
		}
	}
}

MicroBuf::~MicroBuf() {
	// TODO Auto-generated destructor stub
}

}
