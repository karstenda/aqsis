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

#ifndef MICROBUF_H_
#define MICROBUF_H_


#include <cfloat>
#include <cmath>
#include <cstring>

#include <boost/scoped_array.hpp>

#include <OpenEXR/ImathMath.h>
#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathColor.h>


namespace Aqsis {

inline float dot(Imath::V3f a, Imath::V3f b) {
	return a ^ b;
}


/// An axis-aligned cube environment buffer.
///
/// Each face has a coordinate system where the centres of the boundary pixels
/// lie just _inside_ the boundary.  That is, the positions of the pixel point
/// samples are at x_i = (1/2 + i/N) for i = 0 to N-1.
///
/// For example, with 3x3 pixel faces, each
/// face looks like
///
///   +-----------+
///   | x   x   x |
///   |           |
///   | x   x   x |
///   |           |
///   | x   x   x |
///   +-----------+
///
/// where the x's represent the positions at which point sampling will occur.
///
/// The orientation of the faces is chosen so that they have a consistent
/// coordinate system when laid out in the following unfolded net of faces,
/// viewed from the inside of the cube.  (NB that for other possible nets the
/// coordinates of neighbouring faces won't be consistent.)
///
///              +---+
///   ^          |+y |
///   |  +---+---+---+---+
///  v|  |-z |-x |+z |+x |
///   |  +---+---+---+---+
///   |          |-y |
///   |          +---+
///   |     u
///   +-------->
///
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


///
class MicroBuf {

public:

	static const Imath::V3f Normal;

private:

	/// Square face resolution
	int m_res;
	/// Number of channels per pixel
	int m_nchans;
	/// Number of floats needed to store a face
	int m_faceSize;
	/// Pixel face storage
	boost::scoped_array<float> m_pixels;
	boost::scoped_array<float> m_defaultPixels;
	/// Storage for pixel ray directions
	boost::scoped_array<Imath::V3f> m_directions;
	/// Pixels on a unit cube are not all equal in angular size
	boost::scoped_array<float> m_pixelSizes;

public:

	/// Identifiers for each cube face direction
	enum Face {
		Face_xp, ///< x+
		Face_yp, ///< y+
		Face_zp, ///< z+
		Face_xn, ///< x-
		Face_yn, ///< y-
		Face_zn, ///< z-
		Face_end,
		Face_begin = Face_xp
	};

	/// faceRes gives face resolution.  Faces are square.  nchans gives
	/// the number of channels in each pixel, and defaultPix gives the
	/// values of the channels which will be used when reset() is called.
	MicroBuf(int faceRes, int nchans, const float* defaultPix);

	MicroBuf(MicroBuf& microbuf);

	void reset(float* pixels);

	/// Reset buffer to default (non-rendered) state.
	void reset();

	/// Get raw data store for face
	float* face(int which);
	const float* face(int which) const;

	/// Get index of face which direction p sits inside.
	static Face faceIndex(Imath::V3f p);

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
	static Face neighbourU(int faceIdx, int side);

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
	static Face neighbourV(int faceIdx, int side);

	/// Get coordinates on face
	///
	/// The coordinates are in the range -1 <= u,v <= 1, if faceIdx is
	/// obtained using the faceIndex function.  Coordinates outside this
	/// range are legal, as long as p has nonzero component in the
	/// direction of the normal of the face.
	///
	/// \param faceIdx - index of current face
	/// \param p - position (may lie outside cone of current face)
	static void faceCoords(int faceIdx, Imath::V3f p, float& u, float& v);

	/// Compute dot product of vec with face normal on given face
	static float dotFaceNormal(int faceIdx, Imath::V3f vec);

	/// Compute face normal
	static Imath::V3f faceNormal(int faceIdx);

	/// Get direction vector for pixel on given face.
	Imath::V3f rayDirection(int faceIdx, int u, int v) const;

	/// Return relative size of pixel.
	///
	/// Compared to a pixel in the middle of the cube face, pixels in the
	/// corners of the cube have a smaller angular size.  We must take
	/// this into account when integrating the radiosity.
	///
	/// \param u,v - face coordinates
	float pixelSize(int u, int v) const;

	/// Reorder vector components into "canonical face coordinates".
	///
	/// The canonical coordinates correspond to the coordinates on the +z
	/// face.  If we let the returned vector be q then (q.x, q.y)
	/// correspond to the face (u, v) coordinates, and q.z corresponds to
	/// the signed depth out from the face.
	static Imath::V3f canonicalFaceCoords(int faceIdx, Imath::V3f p);

	Imath::C3f getRadiosityInDir(const Imath::V3f direction) const;

	Imath::C3f getInterpolatedRadiosityInDir(const Imath::V3f direction) const;

	float* getPixelPointer();

	int getFaceResolution() const;

	int getNChans() const;

	/// Face side resolution
	int res() const;

	/// Number of channels per pixel
	int nchans() const;

	/// Total size of all faces in number of texels
	int size() const;

	virtual ~MicroBuf();

private:
	/// Get direction vector for position on a given face.
	///
	/// Roughly speaking, this is the opposite of the faceCoords function
	static Imath::V3f direction(int faceIdx, float u, float v);

};

}
#endif /* MICROBUF_H_ */
