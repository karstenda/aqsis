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

#include <aqsis/util/logging.h>

#include "RadiosityIntegrator.h"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

RadiosityIntegrator::RadiosityIntegrator(int faceRes):
		m_buf(faceRes, 5, defaultPixel()),
		m_face(0),
		m_currRadiosity(0) {

	clear();
}

RadiosityIntegrator::RadiosityIntegrator(MicroBuf& microbuffer):
		m_buf(microbuffer),
		m_face(0),
		m_currRadiosity(0) {

}

/// Get direction of the ray
V3f RadiosityIntegrator::rayDirection(int iface, int u, int v) {
	return m_buf.rayDirection(iface, u, v);
}

/// Get at the underlying buffer
MicroBuf& RadiosityIntegrator::microBuf() {
	return m_buf;
}

/// Get desired resolution of environment map faces
int RadiosityIntegrator::res() const {
	return m_buf.res();
}

/// Reset buffer to default state
void RadiosityIntegrator::clear() {
	m_buf.reset();
}
;

/// Set extra data (eg, radiosity) associated with the current point
/// being shaded.
void RadiosityIntegrator::setPointData(const float* radiosity) {
	m_currRadiosity = C3f(radiosity[0], radiosity[1], radiosity[2]);
}

/// Set the face to which subsequent calls of addSample will apply
void RadiosityIntegrator::setFace(int iface) {
	m_face = m_buf.face(iface);
}
;

/// Add a rasterized sample to the current face
///
/// \param u,v - coordinates of face
/// \param distance - distance to sample
/// \param coverage - estimate of pixel coverage due to this sample
void RadiosityIntegrator::addSample(int u, int v, float distance, float coverage) {
	float* pix = m_face + (v * m_buf.res() + u) * m_buf.nchans();
	// TODO: Eventually remove dist if not needed
	float& currDist = pix[0];
	float& currCover = pix[1];
	C3f & radiosity = *reinterpret_cast<C3f*> (pix + 2);
	if (distance < currDist)
		currDist = distance;
	if (currCover < 1) {
		if (currCover + coverage <= 1) {
			radiosity += coverage * m_currRadiosity;
			currCover += coverage;
		} else {
			radiosity += (1 - currCover) * m_currRadiosity;
			currCover = 1;
		}
	}
}

/// Integrate radiosity based on previously sampled scene.
///
/// N is the shading normal; coneAngle is the angle over which to
/// consider the occlusion in radians.  If coneAngle is not PI/2
/// radians, the usual cos(theta) weighting is adjusted to
/// (cos(theta) - cos(coneAngle)) so that it falls continuously to
/// zero when theta == coneAngle
///
/// If occlusion is non-null, the amount of occlusion will also be
/// computed and stored.
C3f RadiosityIntegrator::radiosity(V3f N, float coneAngle, float* occlusion) const {
	// Integrate incoming light with cosine weighting to get outgoing
	// radiosity
	C3f rad(0);
	float totWeight = 0;
	float cosConeAngle = std::cos(coneAngle);
	float occ = 0;
	for (int f = MicroBuf::Face_begin; f < MicroBuf::Face_end; ++f) {
		const float* face = m_buf.face(f);
		for (int iv = 0; iv < m_buf.res(); ++iv)
			for (int iu = 0; iu < m_buf.res(); ++iu, face += m_buf.nchans()) {
				float d = dot(m_buf.rayDirection(f, iu, iv), N) - cosConeAngle;
				if (d > 0) {
					d *= m_buf.pixelSize(iu, iv);
					C3f & radiosity = *(C3f*) (face + 2);
					rad += d * radiosity;
					occ += d * face[1];
					totWeight += d;
				}
			}
	}
	if (totWeight != 0) {
		occ /= totWeight;
		rad = (1.0f / totWeight) * rad;
	}
	if (occlusion)
		*occlusion = occ;
	return rad;
}

/// Integrate radiosity based on previously sampled scene using phong sampling.
///
/// N is the shading normal; I is the incoming viewing direction.
/// PhongExponent is the exponent used for the phong sampling.
///
/// If occlusion is non-null, the amount of occlusion will also be
/// computed and stored.
C3f RadiosityIntegrator::phongRadiosity(V3f N, V3f I, int phongExponent, float* occlusion) const {

	if (phongExponent <= 0) {
		return radiosity(N, M_PI_2, occlusion);
	}

	//Transform from world space to microbuffer space
	V3f diff = MicroBuf::Normal - N;
	I = I + diff;

	C3f rad(0);
	float totWeight = 0;
	float occ = 0;
	N = N.normalized();
	I = I.normalized();

	// Calculating the reflected ray
	// I - 2 * N * ( DotProduct[ I,N] )

	V3f R = I - 2 * (dot(I, N)) * N;

	for (int f = MicroBuf::Face_begin; f < MicroBuf::Face_end; ++f) {
		const float* face = m_buf.face(f);
		for (int iv = 0; iv < m_buf.res(); ++iv)
			for (int iu = 0; iu < m_buf.res(); ++iu, face += m_buf.nchans()) {
				V3f direction = m_buf.rayDirection(f, iu, iv);

				float cos = dot(direction, N);
				if (cos > 0) {
					float size = m_buf.pixelSize(iu, iv);
					// Calculate phong factor
					float phongFactor = pow(std::max(0.0f, dot(R, direction)),phongExponent);
					float scale = phongFactor * size * cos;

					C3f& radiosity = *(C3f*) (face + 2);
					rad += scale * radiosity;
					occ += scale * face[1];




					totWeight += scale;
				}
			}
	}
	if (totWeight != 0) {
		occ /= totWeight;
		rad = (1.0f / totWeight) * rad;
	}
	if (occlusion)
		*occlusion = occ;


	if (isnan(rad.x) || isinf(rad.x) || isinf(-rad.x)) {
		rad.x = 0;
	}
	if (isnan(rad.y) || isinf(rad.y) || isinf(-rad.y)) {
		rad.y = 0;
	}
	if (isnan(rad.z) || isinf(rad.z) || isinf(-rad.z)) {
		rad.z = 0;
	}


	return rad;
}


RadiosityIntegrator::~RadiosityIntegrator() {

}

}
