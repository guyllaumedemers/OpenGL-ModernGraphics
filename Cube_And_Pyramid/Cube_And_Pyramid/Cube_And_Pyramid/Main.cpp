#pragma once
#include "Utilities.h"

#define numVAOs 1
#define numVBOs 2
#define cubeArrLength 108
#define pyramidArrLength 54


GLuint vao[numVAOs];
GLuint vbo[numVBOs];

const char* uniform_modelview_matrix = "mv_matrix";
const char* uniform_proj_matrix = "proj_matrix";

GLint mvLoc, projLoc;

GLuint ren;
const char* vp = "vp.shader";
const char* fp = "fp.shader";

glm::mat4 projMat, viewMat;
glm::vec3 cam;

std::vector<glm::vec3> modelArrPos(numVBOs);

std::vector<GLsizei> modelLengthArray = {
	cubeArrLength,
	pyramidArrLength
};

void SetupVetices() {
	float cubePos[cubeArrLength] = {
		-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

	float pyramidPos[pyramidArrLength] = {
		 -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
	};

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cubeArrLength, cubePos, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * pyramidArrLength, pyramidPos, GL_STATIC_DRAW);
}

int main(int argc, char* argv) {
	GLFWwindow* window = Utilities::CreateWindow(600, 400, "window", 4, 3);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::puts("GLEW Init failed");
		exit(EXIT_FAILURE);
	}

	Utilities::PreGameLoop(window, ren, vp, fp, numVAOs, vao, numVBOs, vbo, SetupVetices, viewMat, projMat, glm::radians(60.0f), 0.1f, 1000.0f, cam, 0.0f, 0.0f, 8.0f, Utilities::SetupModelArrInCameraSpace, modelArrPos);
	Utilities::GameLoop(window, ren, vbo, modelArrPos, modelLengthArray, uniform_modelview_matrix, mvLoc, uniform_proj_matrix, projLoc, projMat, cam);
	Utilities::DestroyWindow(window);
	return 0;
}