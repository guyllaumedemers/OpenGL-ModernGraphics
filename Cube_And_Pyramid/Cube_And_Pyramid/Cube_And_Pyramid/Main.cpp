#pragma once
#include "Utilities.h"

#define numVAOs 1
#define numVBOs 2
#define cubeArrLength 108
#define pyramidArrLength 54


GLuint vao[numVAOs];
GLuint vbo[numVBOs];

const char* mvp_uniform = "mvp_matrix";

GLint mvpLoc;

GLuint rProg;
const char* vp = "vp.shader";
const char* fp = "fp.shader";

glm::mat4 pv_matrix, mvp_matrix;
glm::vec3 cam{ 0.0f, 0.0f, 8.0f };

std::vector<glm::vec3> model_positions{
	glm::vec3{0.0f,-2.0f,0.0f},
	glm::vec3{2.0f,4.0f,0.0f}
};

std::vector<std::vector<float>> models{
	// Cube
	{
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
	},
	// Pyramide
	{
		 -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f
	}
};

int main(int argc, char* argv) {
	GLFWwindow* window = Utilities::CreateWindow(600, 400, "window", 4, 3);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::puts("GLEW Init failed");
		exit(EXIT_FAILURE);
	}

	Utilities::PreGameLoop(window, rProg, vp, fp, pv_matrix, cam, glm::radians(60.0f), 0.1f, 1000.0f, models, numVAOs, vao, numVBOs, vbo);
	Utilities::GameLoop(window, rProg, pv_matrix, model_positions, mvpLoc, mvp_uniform, vbo, models, GL_TRIANGLES);
	Utilities::DestroyWindow(window);
	return 0;
}