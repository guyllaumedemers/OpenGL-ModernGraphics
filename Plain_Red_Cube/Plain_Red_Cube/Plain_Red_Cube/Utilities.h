#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "GLSLToolDebug.h"
#include "GLSLFileReader.h"

namespace Utilities {
#if Matrix
#endif
	glm::mat4 PerspectiveMat(GLFWwindow* window, const float& rad, const float& zNear, const float& zFar) {
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		return glm::perspective(rad, (float)width / (float)height, zNear, zFar);
	}

	glm::mat4 MvMat(glm::vec3& cam, glm::vec3& model) {
		return glm::translate(glm::mat4(1.0f), cam) * glm::translate(glm::mat4(1.0f), model);
	}

#if ShaderProgram
#endif
	// Take a vertex Shader and a fragment shader as args
	GLuint createShaderProgram(const char* vp, const char* fp) {
		GLint vertCompiled;
		GLint fragCompiled;
		GLint linked;

		std::string vShaderStr = GLSLFileReader::ReadShaderSource(vp);		// cannot convert directly to c_Str() since it destroy the temporary object
		std::string fShaderStr = GLSLFileReader::ReadShaderSource(fp);

		const char* vShaderSource = vShaderStr.c_str();
		const char* fShaderSource = fShaderStr.c_str();

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vShader, 1, &vShaderSource, NULL);
		glShaderSource(fShader, 1, &fShaderSource, NULL);

		glCompileShader(vShader);
		GLSLToolDebug::CheckOpenGLError();
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);			// reference his adress so we can fill his value
		if (vertCompiled != 1) {
			std::cout << "vertex compilation failed" << std::endl;
			GLSLToolDebug::PrintShaderLog(vShader);
		}

		glCompileShader(fShader);
		GLSLToolDebug::CheckOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1) {
			std::cout << "vertex compilation failed" << std::endl;
			GLSLToolDebug::PrintShaderLog(fShader);
		}

		GLuint vfProgram = glCreateProgram();

		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);

		glLinkProgram(vfProgram);
		GLSLToolDebug::CheckOpenGLError();
		glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
		if (linked != 1) {
			std::cout << "linking failed" << std::endl;
			GLSLToolDebug::PrintProgramLog(vfProgram);
		}

		return vfProgram;
	}

	// Take a vertex Shader, a geometry shader and a fragment shader as args
	GLuint createShaderProgram(const char* vp, const char* gp, const char* fp) {
		// fill in later
		return 0;
	}

	// Take a vertex Shader, tessellation and a fragment shader as args
	GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp) {
		// fill in later
		return 0;
	}

	// Take a vertex Shader, tessellation, a geometry shader and a fragment shader as args
	GLuint createShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp) {
		// fill in later
		return 0;
	}

#if main
#endif
	GLFWwindow* CreateWindow(const float& width, const float& height, const char* name, const float& major_version, const float& minor_version) {
		if (!glfwInit()) {
			std::puts("GLFW lib not initialize");
			exit(EXIT_FAILURE);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);

		return glfwCreateWindow(width, height, name, NULL, NULL);
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

	void init(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const int& numVAOs, GLuint vao[], const int& numVBOs, GLuint vbo[], float model[]) {
		rProg = Utilities::createShaderProgram(vp, fp);
		// camera setup
		SetupVertexArr(numVAOs, vao, numVBOs, vbo, model, 0);
	}

	void Refresh(GLuint& program) {
		glClear(GL_DEPTH_BUFFER);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
	}

	void display(GLFWwindow* window, const double& currentTime, const char* mv, GLuint& mvLoc, const char* proj, GLuint& projLoc, GLuint& rProg,
		glm::mat4& mvMat, glm::mat4& perpMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, glm::vec3& model)
	{
		Refresh(rProg);
		mvLoc = glGetUniformLocation(rProg, mv);
		projLoc = glGetUniformLocation(rProg, proj);
		perpMat = PerspectiveMat(window, rad, zNear, zFar);
		mvMat = MvMat(cam, model);
		// need to link both matrix to uniform in shader prog
		// associate VBO with vertex attrib
		// adjust OpenGL setting / draw model
	}

	void PreGameLoop(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const int& numVAOs, GLuint vao[], const int& numVBOs, GLuint vbo[], float model[]) {
		glfwSwapInterval(1);
		init(window, rProg, vp, fp, numVAOs, vao, numVBOs, vbo, model);
	}

	void GameLoop(GLFWwindow* window, const double& currentTime, const char* mv, GLuint& mvLoc, const char* proj, GLuint& projLoc, GLuint& rProg,
		glm::mat4& mvMat, glm::mat4& perpMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, glm::vec3& model)
	{
		while (!glfwWindowShouldClose(window)) {
			display(window, currentTime, mv, mvLoc, proj, projLoc, rProg, mvMat, perpMat, rad, zNear, zFar, cam, model);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}