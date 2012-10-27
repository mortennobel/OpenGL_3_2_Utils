/*!
 * OpenGL 3.2 Utils - Extension to the Angel library (from the book Interactive Computer Graphics 6th ed
 * https://github.com/mortennobel/OpenGL_3_2_Utils
 *
 * New BSD License
 *
 * Copyright (c) 2011, Morten Nobel-Joergensen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "NURBS.h"

#include <cassert>
#include <algorithm>

using namespace std;

NURBS::NURBS(void)
{
}


NURBS::~NURBS(void)
{
}

bool NURBS::isZeroFunction(int knotIndex,  int degree, std::vector<float>  const &knotVector){
	if (degree >0){
		return isZeroFunction(knotIndex, degree-1, knotVector) && isZeroFunction(knotIndex+1, degree-1, knotVector);
	}
	assert(knotIndex>=0);
	assert(knotIndex<knotVector.size()-1);
	int knotSize = knotVector.size();
	int multiplicity = 0;
	if (knotVector[knotIndex] == knotVector[knotIndex+1]){
		for (int i = knotIndex - 1;i>=0 && knotVector[i] == knotVector[knotIndex]; i--){
			multiplicity++;
		}
		for (int i = knotIndex + 2;i<knotSize && knotVector[i] == knotVector[knotIndex]; i++){
			multiplicity++;
		}
	}
	return multiplicity>degree;
}

float NURBS::basisFunction(int knotIndex, int degree, float u, std::vector<float>  const &knotVector) {
	assert(degree >= 0);
	
	// based on http://mathworld.wolfram.com/B-Spline.html
	if (degree == 0) {
		return knotVector[knotIndex] <= u && u < knotVector[knotIndex+1] ? 1.0f : 0.0f;
	} else {
		float divisor1 = (knotVector[knotIndex + degree  ] - knotVector[knotIndex  ]);
		float divisor2 = (knotVector[knotIndex + degree+1] - knotVector[knotIndex+1]);

		float basisFunctionI0D1 = basisFunction(knotIndex,     degree - 1, u, knotVector);
		float basisFunctionI1D1 = basisFunction(knotIndex + 1, degree - 1, u, knotVector);

		float result = 0;
		if (divisor1 != 0) {
			result += ((u - knotVector[knotIndex]) / divisor1) * basisFunctionI0D1;
		}
		if (divisor2 != 0) {
			result += ((knotVector[knotIndex + degree+1] - u) / divisor2) * basisFunctionI1D1;
		}
		return result;
	}
}