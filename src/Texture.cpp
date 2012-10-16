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

#include "Texture.h"

#include <iostream>

using namespace std;

GLuint Texture::drawTextureShader = 0;
GLuint Texture::drawTextureVertexArrayObject = 0;
GLuint Texture::drawTextureUniform = 0;

Texture::Texture(GLuint width, GLuint height, bool floatingPoint, bool mipmaps )
:width(width), height(height), floatingPoint(floatingPoint){
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, floatingPoint?GL_RGBA32F:GL_RGBA8, width, height, 0,
                 GL_RGBA, floatingPoint?GL_FLOAT:GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if (drawTextureShader == 0){
        setupRenderFullscreenQuad();
    }
}

Texture::~Texture(){
    glDeleteTextures(1, &textureId);
}

void Texture::renderFullscreenQuad(){
    glUseProgram(drawTextureShader);
    glBindVertexArray(drawTextureVertexArrayObject);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(drawTextureUniform, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Texture::setupRenderFullscreenQuad(){
    drawTextureShader = InitShader("fullscreentexture.vert",  "fullscreentexture.frag", "fragColor");
    GLuint positionAttributeLocation = glGetAttribLocation(drawTextureShader, "position");
    drawTextureUniform = glGetUniformLocation(drawTextureShader, "texture");
    
    glGenVertexArrays(1, &drawTextureVertexArrayObject);
    glBindVertexArray(drawTextureVertexArrayObject);
    
    vec2 array[3] = { // triangle fill screen in clip space
        vec2(-1,-1),
        vec2(3,-1),
        vec2(-1,3)
    };
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec2), array[0], GL_STATIC_DRAW);
    
	
	glEnableVertexAttribArray(positionAttributeLocation);
	glVertexAttribPointer(positionAttributeLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (const GLvoid *)(0));
}

void Texture::generateMipmaps(){
    glBindTexture(GL_TEXTURE_2D, textureId);
    glGenerateMipmap(GL_TEXTURE_2D);
}

