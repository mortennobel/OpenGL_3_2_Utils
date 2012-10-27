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

#ifndef _NURBS_H
#define _NURBS_H

#include <vector>
#include "Angel.h"

struct NURBSVector {
	vec4 position;
	vec3 normal; // only used for nurbs surface
	vec2 uv;
};

class NURBS
{
public:
	NURBS(void);
	virtual ~NURBS(void);

	// returns a (unsorted) list of controlpoints
	virtual std::vector<vec4> getControlPoints() = 0;
	virtual std::vector<NURBSVector> getMeshData() = 0;
	virtual std::vector<GLuint> getMeshDataIndices() = 0;

	// evaluate the point based on uv (between 0 and 1). Note that the v parameter is only used for the NURBSSurface.
	virtual vec4 evaluate(float u, float v = 0) = 0;

	virtual GLenum getPrimitiveType() = 0;

	float basisFunction(int knotIndex, int degree, float u, std::vector<float> const &knotVector);
	
	bool isZeroFunction(int knotIndex, int degree, std::vector<float> const &knotVector);	

	// helper functions
	inline bool isNan(float value)
	{
		return value != value;
	}

	inline bool isInf(float value)
	{
		return std::numeric_limits<float>::has_infinity &&
			value == std::numeric_limits<float>::infinity();
	}
};

#endif //  _NURBS_H
