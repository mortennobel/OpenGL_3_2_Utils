#include "NURBSRenderer.h"

using namespace std;

GLuint NURBSRenderer::shaderProgram = 0;
GLuint NURBSRenderer::projectionUniform = 0;
GLuint NURBSRenderer::modelViewUniform = 0;
GLuint NURBSRenderer::lightPositionUniform = 0;
GLuint NURBSRenderer::colorUniform = 0;

GLuint NURBSRenderer::positionAttribute = 0; 
GLuint NURBSRenderer::normalAttribute = 0; 
GLuint NURBSRenderer::uvAttribute = 0;

NURBSRenderer::NURBSRenderer(NURBS * nurbs) 
	: nurbs(nurbs), vao(0), color(1,0,0,1) {

	if (shaderProgram == 0){
		setupShader();
	}
	reloadData();
}

NURBSRenderer::~NURBSRenderer() {
}

void NURBSRenderer::setupShader(){
	shaderProgram = InitShader("nurbs.vert",  "nurbs.frag", "fragColour");
	projectionUniform = glGetUniformLocation(shaderProgram, "projection");
	if (projectionUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'projection' uniform."<<endl;
	}
	modelViewUniform = glGetUniformLocation(shaderProgram, "modelView");
	if (modelViewUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'modelView' uniform."<<endl;
	}
	lightPositionUniform = glGetUniformLocation(shaderProgram, "lightPosition");
	if (lightPositionUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'lightPosition' uniform."<<endl;
	}
	colorUniform = glGetUniformLocation(shaderProgram, "color");
	if (modelViewUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'color' uniform."<<endl;
	}
	positionAttribute = glGetAttribLocation(shaderProgram, "position");
	if (positionAttribute == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'position' attribute." << endl;
	}
	normalAttribute = glGetAttribLocation(shaderProgram, "normal");
	if (normalAttribute == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'normal' attribute." << endl;
	}
	uvAttribute = glGetAttribLocation(shaderProgram, "uv");
	if (uvAttribute == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'uv' attribute." << endl;
	}
}

// set the color of the geometry
void NURBSRenderer::setColor(vec4 color) {
	this->color = color;
}

// get the color of the geometry
vec4 NURBSRenderer::getColor() {
	return color;
}

void NURBSRenderer::renderPoints(mat4 &projection, mat4 &modelView, float pointSize){
	if (vaoControlPoints != 0){
		glUseProgram(shaderProgram);
		glBindVertexArray(vaoControlPoints);
		glPointSize(pointSize);
		vec4 black(0,0,0,1); // this disables any light
		glUniform4fv(colorUniform,1,black);
		glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, projection);
		glUniformMatrix4fv(modelViewUniform, 1, GL_TRUE, modelView);
		glDrawArrays(GL_POINTS,0, controlPointVertexCount);
	}
}

void NURBSRenderer::render(mat4 &projection, mat4 &modelView, vec4 lightPosition){
	if (vao != 0){
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glUniform4fv(colorUniform,1,color);
		glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, projection);
		glUniformMatrix4fv(modelViewUniform, 1, GL_TRUE, modelView);
		glDrawElements(primitiveType,meshDataIndices.size(),GL_UNSIGNED_INT,&(meshDataIndices[0]));
	}
}

void NURBSRenderer::reloadData(){
	primitiveType = nurbs->getPrimitiveType();
	vector<NURBSVector> meshData = nurbs->getMeshData();
	vector<vec4> controlPoints = nurbs->getControlPoints();
	controlPointVertexCount = controlPoints.size();
	for (int i=0;i<controlPointVertexCount;i++){ // set w to 1 when visualizing the control points
		controlPoints[i].w = 1.0; 
	}
	vertexCount = meshData.size();
	meshDataIndices = nurbs->getMeshDataIndices();
	if (meshData.size() == 0){
		return;
	}
	if (vao == 0){
		// surface / curve
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(NURBSVector), meshData[0].position, GL_DYNAMIC_DRAW);
	
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(NURBSVector), (const GLvoid *)0);
		if (normalAttribute != GL_INVALID_INDEX){
			glEnableVertexAttribArray(normalAttribute);
			glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(NURBSVector), (const GLvoid *)(sizeof(vec4)));
		}
		if (uvAttribute != GL_INVALID_INDEX){
			glEnableVertexAttribArray(uvAttribute);
			glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(NURBSVector), (const GLvoid *)(sizeof(vec4)+sizeof(vec3)));
		}

		// control points
		glGenVertexArrays(1, &vaoControlPoints);
		glBindVertexArray(vaoControlPoints);
	
		glGenBuffers(1, &controlPointVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, controlPointVertexBuffer);

		glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(vec4), controlPoints[0], GL_DYNAMIC_DRAW);
	
		glEnableVertexAttribArray(positionAttribute);
		glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), (const GLvoid *)0);
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, meshData.size() * sizeof(NURBSVector), meshData[0].position);

		glBindVertexArray(vaoControlPoints);
		glBufferSubData(GL_ARRAY_BUFFER, 0, controlPoints.size() * sizeof(vec4), controlPoints[0]);
	}
}

