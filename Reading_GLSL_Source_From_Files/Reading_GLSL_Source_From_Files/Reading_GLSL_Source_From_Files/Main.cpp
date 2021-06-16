#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLSLToolDebug.h"
#include "GLSLFileReader.h"

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint CreateShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::string vertShaderStr = GLSLFileReader::ReadShaderSource("vertShader.glsl");
	std::string fragShaderStr = GLSLFileReader::ReadShaderSource("fragShader.glsl");

	const char* vShaderSource = vertShaderStr.c_str();		// retrieve from file
	const char* fShaderSource = fragShaderStr.c_str();

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glShaderSource(fShader, 1, &fShaderSource, NULL);

	glCompileShader(vShader);
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


void Init(GLFWwindow* window) {
	renderingProgram = CreateShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void Display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);
	glDrawArrays(GL_POINTS, 0, 1);
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
	Init(window);
	while (!glfwWindowShouldClose(window)) {
		Display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}