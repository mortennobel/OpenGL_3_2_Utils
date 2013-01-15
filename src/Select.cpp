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
#include "Select.h"

#include <iostream>

using namespace std;

SelectBuffer::SelectBuffer(int width, int height, GLuint colorUniform)
:colorUniform(colorUniform), width(width), height(height)
{
	buildTexture();
	buildFrameBufferObject();
}

SelectBuffer::~SelectBuffer(){
	glDeleteFramebuffers(1, &framebufferObjectId);
	glDeleteRenderbuffers(1, &renderBufferId);
	glDeleteTextures(1,  &textureId);
}

void SelectBuffer::buildTexture() {
	// generate texture
	glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint internalFormat = GL_RGB;
    GLuint format = GL_RGB;
	GLuint storageType = GL_UNSIGNED_BYTE;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
            0, format, storageType, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SelectBuffer::buildFrameBufferObject(){
	glGenFramebuffers(1, &framebufferObjectId);
	glGenRenderbuffers(1, &renderBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjectId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

	GLenum frameBufferRes = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

	cout << getFrameBufferStatusString(frameBufferRes)<<endl;
	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

std::string SelectBuffer::getFrameBufferStatusString(GLenum code){
	switch (code){
        case GL_FRAMEBUFFER_COMPLETE:
            return "Framebuffer ok";
        case GL_FRAMEBUFFER_UNDEFINED:
            return "Framebuffer undefined";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            return "Framebuffer incomplete attachment";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            return "Framebuffer incomplete missing attachment";
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            return "Framebuffer incomplete read buffer";
        case GL_FRAMEBUFFER_UNSUPPORTED:
            return "Framebuffer unsupported";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            return "Framebuffer incomplete multisample";
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            return "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            return "FRAMEBUFFER_INCOMPLETE_FORMATS";
        default:
            return "Unknown framebuffer status ";
    }
}

void SelectBuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjectId);
	glViewport(0,0,width,height);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void SelectBuffer::release(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glFlush();
}

vec4 encode(unsigned short id){
	return vec4((id%256)/256.0f,(id/256)/256.0f,0,0);
}

int decode(GLubyte *bytes){
	return bytes[0]+bytes[1]*256;
}

void SelectBuffer::setId(unsigned short id){
	if (id >= 65535){
		cout << "Warning - id must be less than 65535" << endl;
		id = 65535;
	}
	id++;
	vec4 encodedValue = encode(id);
	glUniform4fv(colorUniform, 1, encodedValue);
}

int SelectBuffer::getId(int x, int y){
	if (x>=width || y >= height || x<0 || y < 0){
		return -1;
	}
	GLubyte pixelData[4] = {0,0,0,0};
	glReadPixels(
 		x,
 		y,
 		1,
 		1,
 		GL_RGB,
 		GL_UNSIGNED_BYTE,
		pixelData);
	return decode(pixelData)-1;
}
