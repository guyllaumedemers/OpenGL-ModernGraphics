#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLSLToolDebug.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

float x = 0.0f;
float delta = 0.01f;
float tWidth = 0.25f;

GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	const char* vShaderSource =
		"#version 430 \n"
		"uniform float offset; \n"
		"void main(void) \n"
		"{ if(gl_VertexID == 0) gl_Position = vec4(0.25 + offset,-0.25, 0, 1); else if(gl_VertexID == 1) gl_Position = vec4(-0.25 + offset,-0.25, 0, 1); else gl_Position = vec4(0.25 + offset,0.25, 0, 1); }";
	const char* fShaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{color = vec4(1,0,0,1);}";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);				// instanciate shader object at null
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vShaderSource, NULL);				// convert shader code to shader object
	glShaderSource(fShader, 1, &fShaderSource, NULL);

	glCompileShader(vShader);										// Compile into low-level machine language	
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		std::cout << "vertex compilation failed" << std::endl;
		GLSLToolDebug::PrintShaderLog(vShader);
	}

	glCompileShader(fShader);
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		GLSLToolDebug::PrintShaderLog(fShader);
	}

	GLuint vfProgram = glCreateProgram();

	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	GLSLToolDebug::CheckOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		GLSLToolDebug::PrintProgramLog(vfProgram);
	}

	return vfProgram;
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);								// useless in this case since this is 2d and there is no depth
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);								// clear the background color each time

	glUseProgram(renderingProgram);								// run program shader

	x += delta;													// increase position of the offset for the triangle
	if (x >= 1.0f - tWidth) delta = -delta;								// swap direction
	else if (x <= -1.0f + tWidth)  delta = -delta;
	GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset");	// get ptr to offset vertex member value
	glProgramUniform1f(renderingProgram, offsetLoc, x);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}

int main(int argc, char* argv) {

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 400, "Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);
	init(window);
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}