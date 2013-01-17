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


#ifndef RADIOSITYINTEGRATOR_H_
#define RADIOSITYINTEGRATOR_H_

#include "MicroBuf.h"

namespace Aqsis {

//------------------------------------------------------------------------------
/// Microbuffer integrator for radiosity
class RadiosityIntegrator {

private:

	static float* defaultPixel() {
		// depth, foreground_coverage, foreground_rgb, background_rgb
		static float def[] = {FLT_MAX, 0, 0, 0, 0, 0, 0, 0};
		return def;
	}

    MicroBuf m_buf;
    float* m_face;
    Imath::C3f m_currRadiosity;


public:
        /// Create integrator with given resolution of the environment map
        /// faces.
        RadiosityIntegrator(int faceRes);

        /// Create the integrator from a given microbuffer.
        RadiosityIntegrator(MicroBuf& microbuffer);

        /// Get direction of the ray
        Imath::V3f rayDirection(int iface, int u, int v);

        /// Get at the underlying buffer
        MicroBuf& microBuf();

        /// Get desired resolution of environment map faces
        int res() const;

        /// Reset buffer to default state
        void clear();

        /// Set extra data (eg, radiosity) associated with the current point
        /// being shaded.
        void setPointData(const float* radiosity);

        /// Set the face to which subsequent calls of addSample will apply
        void setFace(int iface);

        /// Add a rasterized sample to the current face
        ///
        /// \param u,v - coordinates of face
        /// \param distance - distance to sample
        /// \param coverage - estimate of pixel coverage due to this sample
        void addSample(int u, int v, float distance, float coverage);

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
        Imath::C3f radiosity(Imath::V3f N, float coneAngle, float* occlusion = 0) const;

        /// Integrate radiosity based on previously sampled scene using phong sampling.
        ///
        /// N is the shading normal; I is the incoming viewing direction.
        /// PhongExponent is the exponent used for the phong sampling.
        ///
        /// If occlusion is non-null, the amount of occlusion will also be
        /// computed and stored.
        Imath::C3f phongRadiosity(Imath::V3f N, Imath::V3f I, int phongExponent, float* occlusion = 0) const;

        Imath::C3f realRadiosity(Imath::V3f N) const;

        Imath::C3f realPhongRadiosity(Imath::V3f N,  Imath::V3f I, int phongExponent) const;


        /// Destructor
        virtual ~RadiosityIntegrator();

};

}
#endif /* RADIOSITYINTEGRATOR_H_ */
