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