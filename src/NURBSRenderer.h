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

#ifndef _NURBSRenderer_H
#define _NURBSRenderer_H

#include "NURBS.h"

class NURBSRenderer
{
public:
	NURBSRenderer(NURBS * nurbs);
	~NURBSRenderer(void);

	/// reload the nurbs data. Must be called when the NURBS object has been modified.
	void reloadData();

	/// Render the curve using the projection and modelView transforms
	/// If curve, then the light position is ignored
	void render(mat4 &projection, mat4 &modelView, vec4 lightPosition = vec4(0));

	void renderControlPoints(mat4 &projection, mat4 &modelView, float pointSize = 5.0f);

	void renderNormals(mat4 &projection, mat4 &modelView);

	// set the color of the geometry
	void setColor(vec4 color);

	// get the color of the geometry
	vec4 getColor();

	int getVertexCount() { return vertexCount; }
private:
	void setupShader();
	NURBS * nurbs;

	GLuint vao;
	GLuint vertexBuffer;
	int vertexCount;

	GLuint vaoControlPoints;
	GLuint controlPointVertexBuffer;
	int controlPointVertexCount;

	GLuint vaoNormals;
	GLuint normalsVertexBuffer;
	int normalCount;
	
	vec4 color;

	static GLuint shaderProgram;
	static GLuint projectionUniform,
		modelViewUniform,
		lightPositionUniform,
		colorUniform;

	static GLuint positionAttribute, 
		normalAttribute, 
		uvAttribute;

	std::vector<GLuint> meshDataIndices;
	GLenum primitiveType; // lines or triangle strips
};

#endif // _NURBSRenderer_H
