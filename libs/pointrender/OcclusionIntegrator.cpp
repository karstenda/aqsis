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

#include "PointOctree.h"
#include "OcclusionIntegrator.h"

namespace Aqsis {

using Imath::V3f;
using Imath::C3f;

/// Create integrator with given resolution of the environment map
/// faces.
OcclusionIntegrator::OcclusionIntegrator(int faceRes) :
	m_buf(faceRes, 1, defaultPixel()), m_face(0) {
	clear();
}

/// Get direction of the ray
V3f OcclusionIntegrator::rayDirection(int iface, int u, int v) {
	return m_buf.rayDirection(iface, u, v);
}

/// Get at the underlying buffer
const MicroBuf& OcclusionIntegrator::microBuf() {
	return m_buf;
}

/// Get desired resolution of environment map faces
int OcclusionIntegrator::res() {
	return m_buf.res();
}

/// Reset buffer to default state
void OcclusionIntegrator::clear() {
	m_buf.reset();
}

/// Set extra data (eg, radiosity) associated with the current point
/// being shaded.
void OcclusionIntegrator::setPointData(const float*) {
}

/// Set the face to which subsequent calls of addSample will apply
void OcclusionIntegrator::setFace(int iface) {
	m_face = m_buf.face(iface);
}
;

/// Add a rasterized sample to the current face
///
/// \param u,v - coordinates of face
/// \param distance - distance to sample
/// \param coverage - estimate of pixel coverage due to this sample
void OcclusionIntegrator::addSample(int u, int v, float distance,
		float coverage) {
	float* pix = m_face + (v * m_buf.res() + u) * m_buf.nchans();
	// There's more than one way to combine the coverage.
	//
	// 1) The usual method of compositing.  This assumes that
	// successive layers of geometry are uncorrellated so that each
	// attenuates the layer before, but a bunch of semi-covered layers
	// never result in full opacity.
	//
	// 1 - (1 - o1)*(1 - o2);
	//
	// 2) Add the opacities (and clamp to 1 at the end).  This is more
	// appropriate if we assume that we have adjacent non-overlapping
	// surfels.
	pix[0] += coverage;
}

/// Compute ambient occlusion based on previously sampled scene.
///
/// This is one minus the zero-bounce light coming from infinity to the
/// point.
///
/// N is the shading normal; coneAngle is the angle over which to
/// consider the occlusion in radians.  If coneAngle is not PI/2
/// radians, the usual cos(theta) weighting is adjusted to
/// (cos(theta) - cos(coneAngle)) so that it falls continuously to
/// zero when theta == coneAngle
float OcclusionIntegrator::occlusion(V3f N, float coneAngle) const {
	// Integrate over face to get occlusion.
	float occ = 0;
	float totWeight = 0;
	float cosConeAngle = std::cos(coneAngle);
	for (int f = MicroBuf::Face_begin; f < MicroBuf::Face_end; ++f) {
		const float* face = m_buf.face(f);
		for (int iv = 0; iv < m_buf.res(); ++iv)
			for (int iu = 0; iu < m_buf.res(); ++iu, face += m_buf.nchans()) {
				float d = dot(m_buf.rayDirection(f, iu, iv), N) - cosConeAngle;
				if (d > 0) {
					d *= m_buf.pixelSize(iu, iv);
					// Accumulate light coming from infinity.
					occ += d * std::min(1.0f, face[0]);
					totWeight += d;
				}
			}
	}
	if (totWeight != 0)
		occ /= totWeight;
	return occ;
}

OcclusionIntegrator::~OcclusionIntegrator() {
	// TODO Auto-generated destructor stub
}

}
