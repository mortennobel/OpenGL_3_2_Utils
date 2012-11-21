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
#ifndef _NURBS_CURVE_H
#define _NURBS_CURVE_H

#include <vector>
#include "Angel.h"
#include "NURBS.h"


/// NURBSCurve represents a NURBS curve.
/// Each curve object must be given a number of control points for each
/// point - the number of control points is specified
/// in the contructor.
/// The curve object also needs a knot vector in a non-decreasing order.
/// The size of the knot vector must be larger than than the number of 
/// control points.
/// The order of the curve is knot vector size minus the number 
/// of control points.
/// A tesselated curve is created using the methods getMeshData() and getMeshDataIndices()
/// An easy way to render the patch is using the NURBSRenderer class.
class NURBSCurve : public NURBS
{
public:
	NURBSCurve(int numberOfControlPoints, int discretization = 1024);
	~NURBSCurve();
	
	/// Set the controlPoint at index 
	void setControlPoint(int index, vec3 controlPoint);

	/// Set the controlPoint at index, the w-component of the controlpoint is the rational value
	void setControlPoint(int index, vec4 controlPoint);
	
	// set the knot vector (used for NonUniformBSplines and NonUniformRationalBSplines (NURBS)
	bool setKnotVector(int count, float const * knotVector);

	// return the number of control points
	int getNumberOfControlPoints();

	// Return the correct size of the knot vector
	int getKnotVectorSize();
	
	// return the order of the curve (degree + 1)
	int getOrder();

	// return the degree of the curve (knotVectorSize - numberOfControlPoints -1)
	int getDegree();

	std::vector<vec4> getControlPoints();

	std::vector<NURBSVertex> getMeshData();
	std::vector<GLuint> getMeshDataIndices();

	// evaluate the point based on u (between 0 and 1). Note that the v parameter is not used here.
	vec4 evaluate(float u, float v = 0);

	// For NURBSCurve always return line_strip
	GLenum getPrimitiveType();
private:
	int degree;
	int numberOfControlPoints;
	std::vector<float> knotVector;
	int discretization;

	vec4 *controlPoints;
};

#endif // _NURBS_CURVE_H
