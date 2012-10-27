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

#include "NURBSCurve.h"

#include <iostream>
#include <cassert>
#include <limits>

using namespace std;

NURBSCurve::NURBSCurve(int numberOfControlPoints, int vertexCount)
:numberOfControlPoints(numberOfControlPoints), vertexCount(vertexCount), degree(-1) {
	controlPoints = new vec4[numberOfControlPoints];
}
	
NURBSCurve::~NURBSCurve(){
	delete [] controlPoints;
}

vector<vec4> NURBSCurve::getControlPoints(){
	vector<vec4> res;
	for (int i=0;i<numberOfControlPoints;i++){
		res.push_back(controlPoints[i]);
	}
	return res;
}
	
void NURBSCurve::setControlPoint(int index, vec3 controlPoint){
	setControlPoint(index, vec4(controlPoint,1.0f));
}

/// Set the controlPoint at index, the w-component of the controlpoint is the rational value
void NURBSCurve::setControlPoint(int index, vec4 controlPoint) {
	assert(index >= 0 && index < numberOfControlPoints);
	controlPoints[index] = controlPoint;
}

bool NURBSCurve::setKnotVector(int knotSize, float const * knotVector){
	float knotValue = knotVector[0];
	for (int i=1;i<knotSize;i++){
		if (knotVector[i] < knotValue){
			cerr << "Error: Knot-vector must be in a non-decreasing order" << endl;
			return false;
		}
		knotValue = knotVector[i];
	}
	degree = knotSize - numberOfControlPoints - 1; 
	
	if (degree < 0){
		cerr << "Error: The knot-vector for " <<numberOfControlPoints<<" control-points must be larger than "<<numberOfControlPoints<< endl;
		return false;
	}
	
	this->knotVector.clear();

	// normalize and copy
	for (int i=0;i<knotSize;i++){
		this->knotVector.push_back(knotVector[i]);
	}

	return true;
}

vector<NURBSVector> NURBSCurve::getMeshData(){
	vector<NURBSVector> res;
	if (degree>=0){
		float min = knotVector[degree];
		float max = knotVector[knotVector.size()-1-degree];
		float delta = (max-min)*0.99999f; // avoid using max value since basis function is not included

		for (int i=0;i<vertexCount;i++){
			float u = i/float(vertexCount-1);
			u = min + u*delta;
			NURBSVector v;
			v.position = evaluate(u);
			v.uv = vec2(u,u);
			res.push_back(v);
		}
	} else {
		cerr << "Invalid knot vector"<<endl;
	}
	return res;
}

vector<GLuint> NURBSCurve::getMeshDataIndices(){
	vector<GLuint> res;
	if (degree>=0){
		for (int i=0;i<vertexCount;i++){
			res.push_back(i);
		}
	}
	return res;
}

vec4 NURBSCurve::evaluate(float u, float v){
	vec3 res;
	float delimeter = 0;
	
	for (int i=0;i < numberOfControlPoints ; i++){
		vec4 controlPoint = controlPoints[i];
		float val = controlPoint.w * basisFunction(i, degree, u, knotVector);
		assert(!isNan(val));// check for NAN
		assert(! isInf(val));
		res += vec3(controlPoint.x * val, controlPoint.y * val, controlPoint.z * val);

		delimeter += val;
	}
	
	if (delimeter != 0.0){
		res = res / delimeter;
	} 

	return vec4(res, 1.0f);
}

GLenum NURBSCurve::getPrimitiveType() { 
	return  GL_LINE_STRIP; 
}

int NURBSCurve::getNumberOfControlPoints(){
	return numberOfControlPoints;
}

int NURBSCurve::getOrder(){ 
	return degree + 1; 
}

int NURBSCurve::getDegree() { 
	return degree; 
}

int NURBSCurve::getKnotVectorSize(){
	return knotVector.size();
}
