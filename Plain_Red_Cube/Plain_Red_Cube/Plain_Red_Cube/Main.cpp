#pragma once
#include "Utilities.h"

#define WIDTH 600
#define HEIGHT 400
#define MAJOR 4
#define MINOR 3

#define numVAOs 1
#define numVBOs 2

GLuint renProg;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

float vertexArr[108] = {};

glm::mat4 mvMat, perpMat;
glm::vec3 cam, model;
GLuint mvLoc, projLoc;

const char* vp = "vp.shader";
const char* fp = "fp.shader";

const char* mv_matrix = "mv_matrix";
const char* proj_matrix = "proj_matrix";


int main(int argc, char* argv) {
	GLFWwindow* window = Utilities::CreateWindow(WIDTH, HEIGHT, "Window", MAJOR, MINOR);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		std::puts("GLEW lib not initialize");
		exit(EXIT_FAILURE);
	}
	Utilities::PreGameLoop(window, renProg, vp, fp, numVAOs, vao, numVBOs, vbo, vertexArr, cam, 0.0f, 0.0f, 8.0f, model, 0.0f, -2.0f, 0.0f);
	Utilities::GameLoop(window, glfwGetTime(), mv_matrix, mvLoc, proj_matrix, projLoc, renProg, mvMat, perpMat, glm::radians(60.0f), 0.1f, 1000.0f, cam, model, vbo, 36);
	Utilities::DestroyWindow(window);
	return 0;
}