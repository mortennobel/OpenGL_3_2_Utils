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

// Select buffer renders object ids into an offscreen buffer that can be used to find
// which object is on a given pixel in screen coordinates.
// The pseudocode for using the Select buffer is as follows:
//
// SelectBuffer *selectBuffer;
// GLuint colorUniform; // should point to the color uniform (vec4) of the current shader
// void renderScene(bool selectModel){
//    for each object o in objectsInScene){ 
//      if (selectModel){
//            selectBuffer->setId(o.id);
//      } else {
//            setColor(o);
//      }
//      renderObject(o);
//    }
// }
// void onMouseClick(int x, int y){
//   selectBuffer->setColorUniform(colorUniform);
//   selectBuffer->bind();
//   renderScene(true);
//   int objectId = getId(x, y);
//   cout << "Found object " << objectId << endl;
//   selectBuffer->release();
// }
//
// The important part of the pseudocode is to call selectBuffer->setId instead of the setting the color (and change the shader). 
//
// Internally the Select buffer uses a FrameBufferObject that writes to a offscrene texture.
// The select buffer needs a passthrough shader to be bound where the output
// color (vec4) can be set using the colorUniform.
class SelectBuffer {
public:
	SelectBuffer(int width, int height, GLuint colorUniform = 0);
	~SelectBuffer();
	// binds the selectBuffer
	void bind();
	// releases the select buffer
	void release();
	// set the id of the next object to be rendered (used only when select buffer is bound)
	void setId(unsigned short id);
	// returns the id of the pixel x,y (or -1 if no object found)
	int getId(int x, int y);
	// change the color uniform of the current bound shader (must be a vec4 uniform which is written directly to fragment shader output).
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