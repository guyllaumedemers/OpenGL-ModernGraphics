#pragma once
#include "Utilities.h"

#define numVAOs 1
#define numVBOs 2
#define vertexArrLength 108

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLint vLoc;
GLint mLoc;
GLint projLoc;
GLint timeLoc;

GLuint renderingProgram;
glm::vec3 cam, modelpos;
glm::mat4 viewMat, modelMat, projMat;

const char* view_matrix = "view_matrix";
const char* model_matrix = "model_matrix";
const char* proj_matrix = "proj_matrix";
const char* uniform_time = "time";

const char* vp = "vp.shader";
const char* fp = "fp.shader";

float vArr[vertexArrLength] = {
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


int main(int argc, char* argv) {
	GLFWwindow* window = Utilities::CreateWindow(600, 400, "window", 4, 3);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::puts("GLEW INIT Failed");
		exit(EXIT_FAILURE);
	}
	Utilities::PreGameLoop(window, renderingProgram, vp, fp, numVAOs, vao, numVBOs, vbo, vArr, vertexArrLength, projMat, glm::radians(60.0f), 0.1f, 1000.0f, cam, 0.0f, 0.0f, 8.0f, modelpos, 0.0f, -2.0f, 0.0f);
	Utilities::GameLoop(window, renderingProgram, vbo, view_matrix, vLoc, viewMat, model_matrix, mLoc, modelMat, proj_matrix, projLoc, projMat, uniform_time, timeLoc, float(glfwGetTime()), cam, modelpos, 36, 1);
	Utilities::DestroyWindow(window);

	return 0;
}