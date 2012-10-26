#ifndef _NURBS_CURVE_H
#define _NURBS_CURVE_H

#include <vector>
#include "Angel.h"
#include "NURBS.h"

class NURBSCurve : public NURBS
{
public:
	NURBSCurve(int numberOfControlPoints, int vertexCount = 1024);
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

	std::vector<NURBSVector> getMeshData();
	std::vector<GLuint> getMeshDataIndices();

	// evaluate the point based on u (between 0 and 1). Note that the v parameter is not used here.
	vec4 evaluate(float u, float v = 0);

	GLenum getPrimitiveType();
private:
	int degree;
	int numberOfControlPoints;
	std::vector<float> knotVector;
	int vertexCount;

	vec4 *controlPoints;
};

#endif // _NURBS_CURVE_H
