#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLSLToolDebug.h"

#define numVAOs 3

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	const char* vShaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{ if(gl_VertexID == 0) gl_Position = vec4(0.25,-0.25, 0, 1); else if(gl_VertexID == 1) gl_Position = vec4(-0.25,-0.25, 0, 1); else gl_Position = vec4(0.25,0.25, 0, 1); }";
	const char* fShaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{color = vec4(1,0,0,1);}";

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader, 1, &vShaderSource, NULL);
	glShaderSource(fShader, 1, &fShaderSource, NULL);

	glCompileShader(vshader);
	GLSLToolDebug::CheckOpenGLError();									
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {											
		std::cout << "vertex compilation failed" << std::endl;
		GLSLToolDebug::PrintShaderLog(vshader);
	}

	glCompileShader(fShader);
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		std::cout << "fragment compilation failed" << std::endl;
		GLSLToolDebug::PrintShaderLog(fShader);
	}

	GLuint vfProfram = glCreateProgram();

	glAttachShader(vfProfram, vshader);
	glAttachShader(vfProfram, fShader);

	glLinkProgram(vfProfram);
	GLSLToolDebug::CheckOpenGLError();
	glGetProgramiv(vfProfram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed" << std::endl;
		GLSLToolDebug::PrintProgramLog(vfProfram);
	}

	return vfProfram;
}


void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);
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