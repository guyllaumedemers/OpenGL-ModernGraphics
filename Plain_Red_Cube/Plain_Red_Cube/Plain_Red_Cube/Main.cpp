#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#include "Utilities.h"

#define WIDTH 600
#define HEIGHT 400
#define MAJOR 4
#define MINOR 3

#define numVAOs 1
#define numVBOs 2

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

float vertexArr[108] = {};

GLFWwindow* CreateWindow(const float& width, const float& height, const char* name, const float& major_version, const float& minor_version) {
	if (!glfwInit()) {
		std::puts("GLFW lib not initialize");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);

	return glfwCreateWindow(width, height, name, NULL, NULL);
}

void GameLoop(GLFWwindow* window, void (*init)(GLFWwindow* window), void (*display)(GLFWwindow* window, double currentTime)) {
	glfwSwapInterval(1);
	init(window);
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void DestroyWindow(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void SetupVertexArr(const float& numVAO, GLuint vao[], const float& numVBO, GLuint vbo[], float vArr[], int bIndex) {
	glGenVertexArrays(numVAO, vao);
	glBindVertexArray(vao[bIndex]);
	glGenBuffers(numVBO, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[bIndex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vArr), vArr, GL_STATIC_DRAW);
}

void init(GLFWwindow* window, const char* vp, const char* fp, float model[]) {			// not sure if I want to pass everything as args OR set it manually each time...
	renderingProgram = Utilities::createShaderProgram(vp, fp);
	// camera setup
	SetupVertexArr(numVAOs, vao, numVBOs, vbo, model, 0);
}

void display(GLFWwindow* window, double currentTime) {

}

int main(int argc, char* argv) {
	GLFWwindow* window = CreateWindow(WIDTH, HEIGHT, "Window", MAJOR, MINOR);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::puts("GLEW lib not initialize");
		exit(EXIT_FAILURE);
	}
	GameLoop(window, &init, &display);
	DestroyWindow(window);
	return 0;
}