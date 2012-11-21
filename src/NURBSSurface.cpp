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
#include "NURBSSurface.h"

#include <iostream>
#include <cassert>

using namespace std;

NURBSSurface::NURBSSurface(int numberOfControlPointsU, int numberOfControlPointsV, int discretizationU, int discretizationV)
	:numberOfControlPointsU(numberOfControlPointsU),
	 numberOfControlPointsV(numberOfControlPointsV),
	 discretizationU(discretizationU),
	 discretizationV(discretizationV),
	 degreeU(-1),
	 degreeV(-1)
{
	controlPoints = new vec4*[numberOfControlPointsU];
	for (int i = 0;i < numberOfControlPointsU; i++){
		controlPoints[i] = new vec4[numberOfControlPointsV];
	}
}

NURBSSurface::~NURBSSurface() {
	for (int i=0;i<numberOfControlPointsU;i++){
		delete [] controlPoints[i];
	}
	delete [] controlPoints;
}

std::vector<vec4> NURBSSurface::getControlPoints(){
	std::vector<vec4> res;

	for (int i=0;i<numberOfControlPointsU;i++){
		for (int j=0;j<numberOfControlPointsV;j++){
			res.push_back(controlPoints[i][j]);
		}
	}
	return res;
}

void NURBSSurface::setControlPoint(int u, int v, vec3 controlPoint){
	setControlPoint(u,v,vec4(controlPoint, 1.0f));

}

void NURBSSurface::setControlPoint(int u, int v, vec4 controlPoint){
	controlPoints[u][v] = controlPoint;
}

vec4 NURBSSurface::getControlPoint(int u, int v){
	return controlPoints[u][v];
}

bool NURBSSurface::setKnotVector(int knotSize, float const * knotVector, int numberOfControlPoints, int & refDegree, vector<float> & refKnotVector){
	float knotValue = knotVector[0];
	for (int i=1;i<knotSize;i++){
		if (knotVector[i] < knotValue){
			cerr << "Error: Knot-vector must be in a non-decreasing order" << endl;
			return false;
		}
		knotValue = knotVector[i];
	}
	refDegree = knotSize - numberOfControlPoints - 1; 
	
	if (refDegree < 0){
		cerr << "Error: The knot-vector for " <<numberOfControlPoints<<" control-points must be larger than "<<numberOfControlPoints<< endl;
		return false;
	}
	
	refKnotVector.clear();

	// normalize and copy
	float min = knotVector[0];
	float max = knotVector[knotSize-1];
	float delta = max-min;
	for (int i=0;i<knotSize;i++){
		refKnotVector.push_back((knotVector[i] - min)/delta);
	}

	return true;
}

bool NURBSSurface::setKnotVectorU(int count, float const * knotVector){
	return setKnotVector(count, knotVector, numberOfControlPointsU, degreeU, this->knotVectorU);
}

bool NURBSSurface::setKnotVectorV(int count, float const * knotVector){
	return setKnotVector(count, knotVector, numberOfControlPointsV, degreeV, this->knotVectorV);
}

int NURBSSurface::getNumberOfControlPointsU(){
	return numberOfControlPointsU;
}

int NURBSSurface::getNumberOfControlPointsV(){
	return numberOfControlPointsV;
}

int NURBSSurface::getKnotVectorSizeU(){
	return this->knotVectorU.size();
}

int NURBSSurface::getKnotVectorSizeV(){
	return this->knotVectorV.size();
}

int NURBSSurface::getOrderU(){
	return degreeU + 1; 
}

int NURBSSurface::getOrderV(){
	return degreeV + 1; 
}

int NURBSSurface::getDegreeU(){
	return degreeU; 
}

int NURBSSurface::getDegreeV(){
	return degreeV; 
}

std::vector<NURBSVertex> NURBSSurface::getMeshData(){
	vector<NURBSVertex> res;
	if (degreeU < 0 || degreeV < 0){
		return res;
	}
		
	float minU = knotVectorU[degreeU];
	float maxU = knotVectorU[knotVectorU.size()-1-degreeU];
	float deltaU = (maxU - minU) * 0.99999f; // avoid using max value since basis function is not included
	float minV = knotVectorV[degreeV];
	float maxV = knotVectorV[knotVectorV.size()-1-degreeV];
	float deltaV = (maxV - minV) * 0.99999f; // avoid using max value since basis function is not included

	if (degreeU >=0 && degreeV >=0){
		for (int i=0;i<discretizationU;i++){
			for (int j=0;j<discretizationV;j++){
				float u = i/float(discretizationU-1);
				float v = j/float(discretizationV-1);
				u = minU + u*deltaU;
				v = minV + v*deltaV;
				NURBSVertex vertex;
				vertex.position = evaluate(u,v);
				vertex.normal = evaluateNormal(u,v);
				vertex.uv = vec2(u,v);
				res.push_back(vertex);
			}
		}
	} else {
		cerr << "Unvalid knot vector"<<endl;
	}
	return res;
}

int NURBSSurface::getIndex(int u, int v){
	return u*discretizationV+v;
}

std::vector<GLuint> NURBSSurface::getMeshDataIndices(){
	std::vector<GLuint> indices;
	// fill indices
	for(int i = 0; i < discretizationU-1; i++){
		if (i != 0){ // if first vertex, skip degenerate triangle
			// add degenerate triangle
			indices.push_back(getIndex(i+1, 0));
		} 
		for(int j = 0; j < discretizationV;++j)
		{
			indices.push_back(getIndex(i+1, j));
			indices.push_back(getIndex(i, j));
		}
		if (i<discretizationU-2){ // if last vertex skip degenerate triangle
			// add degenerate triangle
			indices.push_back(getIndex(i, discretizationV-1));
		}
	} 
	
	return indices;
}

vec4 NURBSSurface::evaluate(float u, float v){
	vec3 res;
	float delimeter = 0;
	
	for (int i=0;i < numberOfControlPointsU ; i++){
		for (int j=0;j < numberOfControlPointsV ; j++){
			vec4 controlPoint = controlPoints[i][j];
			float basisFunctionU = basisFunction(i, degreeU, u, knotVectorU);
			float basisFunctionV =  basisFunction(j, degreeV, v, knotVectorV);
			float val = controlPoint.w * basisFunctionU * basisFunctionV;
			assert(!isNan(val)); // check for NAN
			assert(!isInf(val));
			res += vec3(controlPoint.x * val, controlPoint.y * val, controlPoint.z * val);

			delimeter += val;
		}
	}
	
	if (delimeter != 0){
		res = res / delimeter;
	} 

	return vec4(res, 1.0f);
}

vec3 NURBSSurface::evaluateNormal(float u, float v){
	// todo - this should be computed analytical instead (!)
	float deltaU =  0.1/float(discretizationU - 1);
	float deltaV =  0.1/float(discretizationV - 1);
	vec4 tangentU = evaluate(min(0.9999f,u+deltaU),v) - evaluate(max(0.0f,u-deltaU),v);
	vec4 tangentV = evaluate(u,min(0.9999f,v+deltaV)) - evaluate(u,max(0.0f,v-deltaV));
	vec3 tangentUv3(tangentU.x,tangentU.y,tangentU.z);
	vec3 tangentVv3(tangentV.x,tangentV.y,tangentV.z);

	return normalize(cross(normalize(tangentVv3), normalize(tangentUv3)));
}

GLenum NURBSSurface::getPrimitiveType(){
	return GL_TRIANGLE_STRIP;
}
