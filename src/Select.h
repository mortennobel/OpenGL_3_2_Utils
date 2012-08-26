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
#ifndef __SELECT_H
#define __SELECT_H

#include <string>
#include <GL/glew.h>
#include "Angel.h"

class SelectBuffer {
public:
	SelectBuffer(int width, int height, GLuint colorUniform = 0);
	~SelectBuffer();
	void bind();
	void release();
	void setId(unsigned short id);
	// returns the id of the pixel x,y (note that 
	int getId(int x, int y);
	void setColorUniform(GLuint colorUniform) { this->colorUniform = colorUniform; }
private:
	void buildTexture();
	void buildFrameBufferObject();
	std::string getFrameBufferStatusString(GLenum code);
	GLuint framebufferObjectId;
	GLuint renderBufferId;
	GLuint textureId;
	GLuint colorUniform;
	int width;
	int height;
};

#endif