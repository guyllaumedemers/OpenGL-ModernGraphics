#include "Utilities.h"

#define numVAOs 1
#define numVBOs 2

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint rProg;
GLuint brickTexture;
GLint mpvLoc;
glm::mat4 proj_mat;
glm::vec3 cam(0.0f, 0.0f, 8.0f);
const char* vp = "vp.shader";
const char* fp = "fp.shader";
const char* texture = "brick1.jpg";
const char* mpv_uniform = "mvp_matrix";

std::vector<std::vector<float>> models{
	// Pyramide
	{
			-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,-1.0f, -1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f,-1.0f
	}
};

std::vector<std::vector<float>> texture_coordinate{
	// Pyramide
	{
			0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,		// top and right faces
			0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,		0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,		// back and left faces
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f		// base triangles
	}
};

std::stack<glm::mat4> mv_stack;

std::vector<glm::vec3> positions{
	glm::vec3(0.0f)
};

std::vector<glm::vec3> rotations{
	glm::vec3(1.0f, 1.0f, 0.0f)
};

std::vector<glm::vec3> scales{
	glm::vec3(2.0f)
};

int main(int argc, char* argv) {
	GLFWwindow* window = Utilities::CreateWindow(600, 400, "window", 4, 3);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::puts("GLEW Lib Init failed");
		exit(EXIT_FAILURE);
	}

	Utilities::PreGameLoop(window, rProg, vp, fp, texture, proj_mat, glm::radians(60.0f), 0.1f, 1000.0f, models, texture_coordinate, brickTexture, numVAOs, vao, numVBOs, vbo);
	Utilities::GameLoop(window, rProg, proj_mat, mv_stack, cam, positions, rotations, scales, mpvLoc, mpv_uniform, vbo, models, GL_TRIANGLES, brickTexture);
	Utilities::DestroyWindow(window);

	return 0;
}