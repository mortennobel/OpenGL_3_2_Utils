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