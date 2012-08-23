#ifndef __OBJ_LOADER_H
#define __OBJ_LOADER_H

#include <vector>
#include "Angel.h"

// Load an OBJ model into the out parameters.
// Note only simple OBJ files are supported.
bool loadObject(const char * filename, 
	std::vector<vec3> &outPositions, 
	std::vector<int> &outIndices,
	std::vector<vec3> &outNormal = std::vector<vec3>(), 
	std::vector<vec2> &outUv = std::vector<vec2>(),
	float scale = 1.0f
	);

#endif
