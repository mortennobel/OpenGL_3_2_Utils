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
