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

#ifndef POINTARRAY_H_
#define POINTARRAY_H_

#include <vector>

#include <OpenEXR/ImathVec.h>
#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathColor.h>

#include "nondiffuse/NonDiffusePoint.h"

namespace Aqsis {


struct NonDiffusePointArray {

	std::vector<NonDiffusePoint> data;

	Imath::V3f centroid() const {
		Imath::V3f centroid(0,0,0);
		for(int i=0; i <data.size(); i++) {
			centroid += data[i].getPosition();
		}
		return (1.0f / data.size())*centroid;
	}

	size_t size() const {
		return data.size();
	}

	void sort() {
		quicksort(data,0,data.size()-1);
	}

	void quicksort(std::vector<NonDiffusePoint> a, const int& leftarg, const int& rightarg) {
		if (leftarg < rightarg) {

			float pivotvalue = a[leftarg].getPosition().length2();

			int left = leftarg - 1;
			int right = rightarg + 1;

			for (;;) {

				while (a[--right].getPosition().length2() > pivotvalue);
				while (a[++left].getPosition().length2() < pivotvalue);

				if (left >= right)
					break;

				NonDiffusePoint temp = a[right];
				a[right] = a[left];
				a[left] = temp;
			}

			int pivot = right;
			quicksort(a, leftarg, pivot);
			quicksort(a, pivot + 1, rightarg);
		}
	}
};


/// Array of surface elements
///
/// TODO: Make into a class, etc.
///
/// Point data is stored in a flat array as
///
///     [P1 N1 r1 user1  P2 N2 r2 user2  ... ]
///
/// where user1 user2... is extra "user data" appended after the position
/// normal, and radius data.
///
struct PointArray {
	int stride;
	std::vector<float> data;

	/// Get number of points in cloud
	size_t size() const {
		return data.size() / stride;
	}

	/// Get centroid of point cloud.
	Imath::V3f centroid() const {
		Imath::V3f sum(0);
		for (std::vector<float>::const_iterator p = data.begin(); p
				< data.end(); p += stride) {
			sum += Imath::V3f(p[0], p[1], p[2]);
		}
		return (1.0f / data.size() * stride) * sum;
	}
};

}
#endif /* POINTARRAY_H_ */
