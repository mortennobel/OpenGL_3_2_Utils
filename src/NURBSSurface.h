#ifndef _NURBS_SURFACE_H
#define _NURBS_SURFACE_H

#include <vector>
#include "Angel.h"
#include "NURBS.h"

class NURBSSurface : public NURBS
{
public:
	NURBSSurface(int numberOfControlPointsU, int numberOfControlPointsV, int vertexCountU = 128, int vertexCountV = 128);
	~NURBSSurface();
	
	std::vector<vec4> getControlPoints();

	/// Set the controlPoint at index 
	void setControlPoint(int u, int v, vec3 controlPoint);

	/// Set the controlPoint at index, the w-component of the controlpoint is the rational value
	void setControlPoint(int u, int v, vec4 controlPoint);

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

	std::vector<NURBSVector> getMeshData();
	std::vector<GLuint> getMeshDataIndices();

	// evaluate the point based on u (between 0 and 1). Note that the v parameter is not used here.
	vec4 evaluate(float u, float v = 0);

	vec3 evaluateNormal(float u, float v);

	GLenum getPrimitiveType();
private:
	bool setKnotVector(int knotSize, float const * knotVector, int numberOfControlPoints, int & refDegree, std::vector<float> & refKnotVector);

	int degreeU;
	int degreeV;
	int numberOfControlPointsU;
	int numberOfControlPointsV;
	std::vector<float> knotVectorU;
	std::vector<float> knotVectorV;
	int vertexCountU;
	int vertexCountV;

	vec4 **controlPoints;
};

#endif // _NURBS_SURFACE_H
