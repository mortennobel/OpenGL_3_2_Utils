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
#ifndef _NURBS_SURFACE_H
#define _NURBS_SURFACE_H

#include <vector>
#include "Angel.h"
#include "NURBS.h"

/// NURBSSurface represents a NURBS surface path.
/// Each surface patch object must be given a number of control points for each
/// point in the u and v direction (the number of control points is specified
/// in the contructor).
/// The surface patch also needs a knot vector in a non-decreasing order for 
/// both the u and v direction. The size of the knot vector must be larger than 
/// than the number of control points (for the given direction).
/// The order of the patch for a given direction is knot vector size minus the number 
/// of control points.
/// A tesselated mesh is created using the methods getMeshData() and getMeshDataIndices()
/// An easy way to render the patch is using the NURBSRenderer class.
class NURBSSurface : public NURBS
{
public:
	NURBSSurface(int numberOfControlPointsU, int numberOfControlPointsV, int discretizationU = 16, int discretizationV = 16);
	~NURBSSurface();
	
	std::vector<vec4> getControlPoints();

	/// Set the controlPoint at index 
	void setControlPoint(int u, int v, vec3 controlPoint);

	/// Set the controlPoint at index, the w-component of the controlpoint is the rational value
	void setControlPoint(int u, int v, vec4 controlPoint);

	// Get the controlPoint at position u, v
	vec4 getControlPoint(int u, int v);
	
	// set the knot vector (used for NonUniformBSplines and NonUniformRationalBSplines (NURBS)
	bool setKnotVectorU(int count, float const * knotVector);
	bool setKnotVectorV(int count, float const * knotVector);

	// return the number of control points
	int getNumberOfControlPointsU();
	int getNumberOfControlPointsV();

	// Return the correct size of the knot vector
	int getKnotVectorSizeU();
	int getKnotVectorSizeV();
	
	// return the order of the curve (degree + 1)
	int getOrderU();
	int getOrderV();

	// return the degree of the curve (knotVectorSize - numberOfControlPoints -1)
	int getDegreeU();
	int getDegreeV();

	// creates a tesselated mesh
	std::vector<NURBSVertex> getMeshData();
	std::vector<GLuint> getMeshDataIndices();

	// evaluate the point based on u (between 0 and 1). Note that the v parameter is not used here.
	vec4 evaluate(float u, float v = 0);

	vec3 evaluateNormal(float u, float v);

	// for NURBS Surface always return triangle strips
	GLenum getPrimitiveType();
private:
	int getIndex(int u, int v);
	bool setKnotVector(int knotSize, float const * knotVector, int numberOfControlPoints, int & refDegree, std::vector<float> & refKnotVector);

	int degreeU;
	int degreeV;
	int numberOfControlPointsU;
	int numberOfControlPointsV;
	std::vector<float> knotVectorU;
	std::vector<float> knotVectorV;
	int discretizationU;
	int discretizationV;

	vec4 **controlPoints;
};

#endif // _NURBS_SURFACE_H
