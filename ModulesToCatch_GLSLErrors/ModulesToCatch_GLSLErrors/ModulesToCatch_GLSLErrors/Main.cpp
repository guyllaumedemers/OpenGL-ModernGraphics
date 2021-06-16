#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLSLToolDebug.h"

#define numVAOs 1							// could be a const, actually a ref to the size of the vertex aray of object

GLuint renderingProgram;					// the rendering program id declaration
GLuint vao[numVAOs];						// the array of vertex declaration => id

GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	const char* vshaderSource =								// vertex shader declaration
		"#version 430 \n"
		"void main(void) \n"
		"{ gl_Position = vec4(0,0,0,1);}";
	const char* fshaderSource =								// fragment shader declaration
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{color = vec4(1,0,0,1);}";

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);		// shader object declaration
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vshader, 1, &vshaderSource, NULL);		// filling the shader object with the GLSL code
	glShaderSource(fshader, 1, &fshaderSource, NULL);

	glCompileShader(vshader);								// convert the shader object to a lower-level language
	// catch error while compiling
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		std::cout << "Vertex compilation failed " << std::endl;
		GLSLToolDebug::PrintShaderLog(vshader);
	}

	glCompileShader(fshader);
	// catch error while compiling
	GLSLToolDebug::CheckOpenGLError();
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		std::cout << "Fragment compilation failed " << std::endl;
		GLSLToolDebug::PrintShaderLog(fshader);
	}

	GLuint vfProgram = glCreateProgram();					// instanciate program

	glAttachShader(vfProgram, vshader);						// attack both shader to program
	glAttachShader(vfProgram, fshader);

	glLinkProgram(vfProgram);								// check if openGL extension compatibility
	GLSLToolDebug::CheckOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		std::cout << "linking failed " << std::endl;
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
	glUseProgram(renderingProgram);
	glPointSize(100.0f);
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