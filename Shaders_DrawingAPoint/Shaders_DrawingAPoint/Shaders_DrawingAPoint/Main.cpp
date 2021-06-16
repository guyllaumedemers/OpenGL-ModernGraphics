#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
	const char* vshaderSource =
		"#version 430 \n"
		"void main(void) \n"
		"{gl_Position = vec4(0,0,0,1);}";
	const char* fshaderSource =
		"#version 430 \n"
		"out vec4 color; \n"
		"void main(void) \n"
		"{ if(gl_FragCoord.x < 300) color = vec4(1,0,0,1); else color = vec4(0,1,0,1);}";

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);						// Create and store shader ID
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);					// which is why we are using GLuint

	glShaderSource(vShader, 1, &vshaderSource, NULL);						// load GLSL code from string into empty shader object
	glShaderSource(fShader, 1, &fshaderSource, NULL);

	glCompileShader(vShader);												// compile into lower-level / machine-code
	glCompileShader(fShader);

	GLuint vfprogram = glCreateProgram();									// instanciate program and store the id of program

	glAttachShader(vfprogram, vShader);										// attach both shaders
	glAttachShader(vfprogram, fShader);

	glLinkProgram(vfprogram);												// request that the GLSL compiler ensure the compatibility of the shaders
	return vfprogram;
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glUseProgram(renderingProgram);													// load program containing both compiled shaders into the OpenGL pipeline onto the GPU
	glPointSize(100.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

int main(int argc, char* argv) {
	if (!glfwInit()) {																// Initialize GLFW lib
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);									// Set version control
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(600, 400, "Window", NULL, NULL);			// Instanciate a window
	glfwMakeContextCurrent(window);													// Set OpenGL context to the calling Thread

	if (glewInit() != GLEW_OK) {													// Initialize the GLEW lib to determine supported openGL extension
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);															// enable VSYNC for when front buffer and back buffer are swap
	init(window);																	// Instanciate the Shader program
	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());												// Display Single vertex from the program
		glfwSwapBuffers(window);													// Swap both buffers
		glfwPollEvents();															// Listen for an event and process it
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}