#include "NURBSSurface.h"

#include <iostream>
#include <cassert>

using namespace std;

NURBSSurface::NURBSSurface(int numberOfControlPointsU, int numberOfControlPointsV, int vertexCountU, int vertexCountV)
	:numberOfControlPointsU(numberOfControlPointsU),
	 numberOfControlPointsV(numberOfControlPointsV),
	 vertexCountU(vertexCountU),
	 vertexCountV(vertexCountV),
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

	if (true){
		vector<NURBSVector> meshData = getMeshData(); // debug !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		for (int i=0;i<meshData.size();i++){
			res.push_back(meshData[i].position);
		}
		return res;
	}

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

std::vector<NURBSVector> NURBSSurface::getMeshData(){
	vector<NURBSVector> res;
	if (degreeU < 0 || degreeV < 0){
		return res;
	}

	for (int i=0;i<numberOfControlPointsU;i++){
		for (int j=0;j<numberOfControlPointsV;j++){
			cout << controlPoints[i][j] << endl;
		}
	}
	
	float minU = knotVectorU[degreeU];
	float maxU = knotVectorU[knotVectorU.size()-1-degreeU];
	float deltaU = (maxU - minU) * 0.99999f; // avoid using max value since basis function is not included
	float minV = knotVectorV[degreeV];
	float maxV = knotVectorV[knotVectorV.size()-1-degreeV];
	float deltaV = (maxV - minV) * 0.99999f; // avoid using max value since basis function is not included

	if (degreeU >=0 && degreeV >=0){
		for (int i=0;i<vertexCountU;i++){
			for (int j=0;j<vertexCountV;j++){
				float u = i/float(vertexCountU-1);
				float v = j/float(vertexCountV-1);
				u = minU + u*deltaU;
				v = minV + v*deltaV;
				NURBSVector vertex;
				vertex.position = evaluate(u,v);
				vertex.normal = evaluateNormal(u,v);
				vertex.uv = vec2(u,v);
				res.push_back(vertex);
				cout << vertex.position << endl;
			}
		}
	} else {
		cerr << "Unvalid knot vector"<<endl;
	}
	return res;
}

std::vector<GLuint> NURBSSurface::getMeshDataIndices(){
	std::vector<GLuint> res;
	int index = 0;
	for (int i=getMeshData().size();i >0 ; i--){	
		res.push_back(index++);		
	}

	return res;
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
	return vec3();
}

GLenum NURBSSurface::getPrimitiveType(){
	return GL_TRIANGLE_STRIP;
}
