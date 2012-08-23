// 02561-01-01 Hello triangle world

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Angel.h"
#include "select.h"

using namespace std;
using namespace Angel;

int WINDOW_WIDTH = 600;
int WINDOW_HEIGHT = 600;

int board[9] = {0,0,0,0,0,0,0,0,0};   /*  color-state for each square	*/

GLuint shaderProgram;
GLuint projectionUniform,
	modelViewUniform,
	colorUniform;
GLuint positionAttribute;
GLuint quadGeometryVAO;

SelectBuffer *selectBuffer = NULL;

typedef struct Vertex{
	vec4 position;
};

const int NUMER_OF_VERTICES = 4;
Vertex vertexData[NUMER_OF_VERTICES] = {
	{ vec4(-0.5, -0.5, 0.0, 1.0 ) },
	{ vec4( 0.5, -0.5, 0.0, 1.0 ) },
	{ vec4( 0.5,  0.5, 0.0, 1.0 ) },
	{ vec4(-0.5,  0.5, 0.0, 1.0 ) }
};

// forward declaration
void loadShader();
void display();
GLuint loadBufferData();
void reshape(int W, int H);

void renderScene(bool select){
	mat4 projection = Ortho(0.0, 3, 0.0, 3, -1.0, 1.0);
	glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, projection);

	glBindVertexArray(quadGeometryVAO);
	int index = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j ++) {
			if (select){
				selectBuffer->setId(index);
			} else {
				vec4 color(i/3.0, j/3.0, board[index]/3.0,1.0);
				glUniform4fv(colorUniform, 1, color);
			}
			mat4 modelView = Translate(0.5+i,0.5+j,0);
			glUniformMatrix4fv(modelViewUniform, 1, GL_TRUE, modelView);
			glDrawArrays(GL_TRIANGLE_FAN, 0, NUMER_OF_VERTICES);
			index++;
		}
	}
}

void display() {	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(shaderProgram);
	
	renderScene(false);
	
	glutSwapBuffers();
}

void reshape(int W, int H) {
	WINDOW_WIDTH = W;
	WINDOW_HEIGHT = H;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (selectBuffer != NULL){
		delete selectBuffer;
	}
	selectBuffer = new SelectBuffer(W,H);
}

GLuint loadBufferData() {
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	GLuint  vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, NUMER_OF_VERTICES * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0);
	return vertexArrayObject;
}

void loadShader() {
	shaderProgram = InitShader("const-shader.vert",  "const-shader.frag", "fragColour");
	projectionUniform = glGetUniformLocation(shaderProgram, "projection");
	if (projectionUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'projection' uniform."<<endl;
	}
	modelViewUniform = glGetUniformLocation(shaderProgram, "modelView");
	if (modelViewUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'modelView' uniform."<<endl;
	}
	colorUniform = glGetUniformLocation(shaderProgram, "color");
	if (colorUniform == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'color' uniform."<<endl;
	}
	positionAttribute = glGetAttribLocation(shaderProgram, "position");
	if (positionAttribute == GL_INVALID_INDEX) {
		cerr << "Shader did not contain the 'position' attribute." << endl;
	}
}


void pickSquares(int button, int state, int x, int y) {
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	selectBuffer->setColorUniform(colorUniform);
	selectBuffer->bind();
	renderScene(true);
	int id = selectBuffer->getId(x,WINDOW_HEIGHT-y);

	selectBuffer->release();
	if (id >= 0){
		board[id] = (board[id] + 1) % 3;
		glutPostRedisplay();
	}
} 

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	
	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);

	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutCreateWindow("02561-04-02");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutMouseFunc (pickSquares);

	glewExperimental = GL_TRUE;  // Added because of http://openglbook.com/glgenvertexarrays-access-violationsegfault-with-glew/
	
	GLint GlewInitResult = glewInit();
	if (GlewInitResult != GLEW_OK) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
	}
	
	loadShader();
	quadGeometryVAO = loadBufferData();

	glutMainLoop();
}