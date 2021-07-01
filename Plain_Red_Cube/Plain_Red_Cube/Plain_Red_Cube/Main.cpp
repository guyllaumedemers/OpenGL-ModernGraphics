#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

#define WIDTH 600
#define HEIGHT 400
#define MAJOR 4
#define MINOR 3

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

void  init(GLFWwindow* window) {

}

void display(GLFWwindow* window, double currentTime) {

}

int main(int argc, char* argv) {
	GLFWwindow* window = CreateWindow(WIDTH, HEIGHT, "Window", MAJOR, MINOR);
	glfwMakeContextCurrent(window);
	GameLoop(window, &init, &display);
	DestroyWindow(window);
	return 0;
}