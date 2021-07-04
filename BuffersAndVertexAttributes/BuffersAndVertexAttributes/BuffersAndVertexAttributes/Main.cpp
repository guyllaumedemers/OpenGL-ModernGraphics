#include <cstdio>				// using stdlib wrapped for c++
#include <string>
#include <GL/glew.h>			// Extension Wrangler lib that query and loads openGL Extensions => determine which extension is compatible
#include <GLFW/glfw3.h>			// window API lib
#include "../OpenGL-Template/header/GLSLFileReader.h"
#include "../OpenGL-Template/header/GLSLToolDebug.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cMath.h"

#define numVAOs 1
#define numVBOs 2

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint renderingProgram;
GLuint mvLoc, projLoc;
glm::mat4 pMat, vMat, mMat, mvMat;
int width, height;
float aspectRatio;
float camX = 0.0f, camY = 0.0f, camZ = 8.0f;
float cubeLocX = 0.0f, cubeLocY = -2.0f, cubeLocZ = 0.0f;
float rad = cMath::degToRad(60);

float vPositions[] = {			// Cube array
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

GLuint CreateProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::string vShaderString = GLSLFileReader::ReadShaderSource("vShaderSource.shader");
	std::string fShaderString = GLSLFileReader::ReadShaderSource("fShaderSource.shader");

	if (vShaderString == "" || fShaderString == "")
	{
		std::printf("Shader String is Empty - VertexString : %s \n FragmentString %s", vShaderString.c_str(), fShaderString.c_str());
		return -1;
	}

	const char* vShaderSource = vShaderString.c_str();
	const char* fShaderSource = fShaderString.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glShaderSource(fShader, 1, &fShaderSource, NULL);

	glCompileShader(vShader);
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != GL_TRUE) {
		std::puts("vertex shader failed");
		GLSLToolDebug::PrintShaderLog(vShader);
		return -1;
	}

	glCompileShader(fShader);
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != GL_TRUE) {
		std::puts("fragment shader failed");
		GLSLToolDebug::PrintShaderLog(fShader);
		return -1;
	}

	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	GLSLToolDebug::CheckOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != GL_TRUE) {
		std::puts("linking failed");
		GLSLToolDebug::PrintProgramLog(vfProgram);
		return -1;
	}

	glDetachShader(vfProgram, vShader);
	glDetachShader(vfProgram, fShader);

	return vfProgram;
}

void init() {
	// initialize the program
	renderingProgram = CreateProgram();
	// generate the VAO
	glGenVertexArrays(numVAOs, vao);											// generate VAO and return id to it => first args represent the number of id to return
	// bind the VAO
	glBindVertexArray(vao[0]);													// set the specified VAO active
	// gen buffer object
	glGenBuffers(numVBOs, vbo);													// generate VBO and return id to it => first args represent the number of id to return
	// bind buffers to vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// load float array into the vbo to display
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions), vPositions, GL_STATIC_DRAW);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);												// clear depth buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);											// set color buffer to black
	glClear(GL_COLOR_BUFFER_BIT);												// clear background to color buffer
	// run program
	glUseProgram(renderingProgram);

	// get the uniform variables for the model-View and projection matrices
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	// build perspective matrix
	glfwGetFramebufferSize(window, &width, &height);
	aspectRatio = (float)width / (float)height;
	pMat = glm::perspective(rad, aspectRatio, 0.1f, 1000.0f);					// camera

	// build view matrix, model matrix and MV
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-camX, -camY, -camZ));						// create an identity matrix and insert the vec3 as A03 A13 A23 A33
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
	mvMat = vMat * mMat;

	// copy perspective matrix and MVmatrix to uniforms to create 3d scene
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));				// send to the uniform ID (mvLoc) the information regarding the model-view matrix created on the C++ side
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));				// same for the projection matrix

	// make buffer active
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);										// make the 0th buffer active
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);						// associate the 0th attribute with buffer
	glEnableVertexAttribArray(0);												// enable the vertex 0th attribute
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// draw array of vertex
	glDrawArrays(GL_TRIANGLES, 0, 36);											// 36 represent the number of triangle in this cube
}


int main(int argc, char* argv[]) {
	if (!glfwInit()) {															// init glfw lib so we can create a window
		std::puts("GLFW Init failed");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);								// specifies the client API to create the context for
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 400, "Window", NULL, NULL);		// Create a window instance
	glfwMakeContextCurrent(window);												// a context represent the default framebuffer that rendering commands will draw to

	if (glewInit() != GLEW_OK) {
		std::puts("GLEW Init failed");
		return -1;
	}

	// Swap interval
	glfwSwapInterval(1);														// swap the interval of the current context, ie number of screen updates to wait / vsync
	// initialize program
	init();
	while (!glfwWindowShouldClose(window)) {
		// display
		display(window, glfwGetTime());
		// swap buffer
		glfwSwapBuffers(window);
		// check for events
		glfwPollEvents();
	}

	// release resources
	glfwDestroyWindow(window);													// destroy the window res
	glfwTerminate();															// Terminate the GLFW lib

	return 0;
}