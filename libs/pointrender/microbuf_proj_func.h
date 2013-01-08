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

#ifndef MICROBUF_PROJ_FUNC_H_
#define MICROBUF_PROJ_FUNC_H_

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathMath.h>

#include "diffuse/DiffusePointOctree.h"
#include "MicroBuf.h"

namespace Aqsis {

//------------------------------------------------------------------------------

/// Render points into a micro environment buffer.
///
/// \param integrator - integrator for incoming geometry/lighting information
/// \param P - position of light probe
/// \param N - normal for light probe (should be normalized)
/// \param coneAngle - defines cone about N: coneAngle = max angle of interest
///                    between N and the incoming light.
/// \param maxSolidAngle - Maximum solid angle allowed for points in interior
///                    tree nodes.
/// \param points - point cloud to render
template<typename IntegratorT>
void microRasterize(IntegratorT& integrator, Imath::V3f P, Imath::V3f N, float coneAngle,
                    float maxSolidAngle, const DiffusePointOctree& points);



/// Rasterize disk into the given integrator
///
/// N is the normal of the culling cone, with cone angle specified by
/// cosConeAngle and sinConeAngle.  The position of the disk with respect to
/// the centre of the microbuffer is p, n is the normal of the disk and r is
/// the disk radius.
template<typename IntegratorT>
void renderDisk(IntegratorT& integrator, Imath::V3f N, Imath::V3f p, Imath::V3f n, float r,
                float cosConeAngle, float sinConeAngle);


}
#endif /* MICROBUF_PROJ_FUNC_H_ */
