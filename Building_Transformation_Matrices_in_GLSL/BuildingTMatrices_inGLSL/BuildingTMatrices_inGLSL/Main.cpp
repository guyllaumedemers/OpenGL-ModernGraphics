#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLSLToolDebug.h"
#include "GLSLFileReader.h"

#define numVAOs 3

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	std::string vShaderString = GLSLFileReader::ReadShaderSource("vShaderSource.shader");
	std::string fShaderString = GLSLFileReader::ReadShaderSource("fShaderSource.shader");

	if (vShaderString == "" || fShaderString == "") {
		std::puts("String file is empty");
		return -1;
	}

	const char* vShaderSource = vShaderString.c_str();
	const char* fShaderSource = fShaderString.c_str();

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