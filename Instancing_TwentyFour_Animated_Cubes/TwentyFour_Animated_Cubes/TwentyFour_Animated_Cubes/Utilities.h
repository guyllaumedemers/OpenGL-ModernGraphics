#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
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
		return glm::translate(glm::mat4(1.0f), -cam) * glm::translate(glm::mat4(1.0f), model);
	}

	glm::mat4 ViewMat(glm::vec3& cam) {
		return glm::translate(glm::mat4(1.0f), -cam);
	}

	glm::mat4 ModelMat(glm::vec3& model) {
		return glm::translate(glm::mat4(1.0f), model);
	}

#if ShaderProgram
#endif
	// Take a vertex Shader and a fragment shader as args
	GLuint CreateShaderProgram(const char* vp, const char* fp) {
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
			std::cout << "fragment compilation failed" << std::endl;
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

		glDetachShader(vfProgram, vShader);
		glDetachShader(vfProgram, fShader);

		return vfProgram;
	}

	// Take a vertex Shader, a geometry shader and a fragment shader as args
	GLuint CreateShaderProgram(const char* vp, const char* gp, const char* fp) {
		// fill in later
		return 0;
	}

	// Take a vertex Shader, tessellation and a fragment shader as args
	GLuint CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* fp) {
		// fill in later
		return 0;
	}

	// Take a vertex Shader, tessellation, a geometry shader and a fragment shader as args
	GLuint CreateShaderProgram(const char* vp, const char* tCS, const char* tES, const char* gp, const char* fp) {
		// fill in later
		return 0;
	}

#if main
#endif
	GLFWwindow* CreateWindow(const int& width, const int& height, const char* name, const int& major_version, const int& minor_version) {
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

	void SetupVertexArr(GLuint vao[], const GLsizei& numVAOs, GLuint vbo[], const GLsizei& numVBOs, float vertexArr[], const GLsizei& vArrLength, const int& bIndex) {
		glGenVertexArrays(numVAOs, vao);
		glBindVertexArray(vao[bIndex]);
		glGenBuffers(numVBOs, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[bIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vArrLength, vertexArr, GL_STATIC_DRAW);
	}

	void SetupBufferArr(GLuint vbo[], const int& bIndex, const int& dimension_size) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[bIndex]);
		glVertexAttribPointer(bIndex, dimension_size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(bIndex);
	}

	void Draw(const GLenum& mode, const GLsizei& polygon_count, const GLsizei& primitive_count) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArraysInstanced(mode, 0, polygon_count, primitive_count);
	}

	void SetupUniformMat(glm::mat4& viewMat, GLint& vLoc, glm::mat4& modelMat, GLint& mLoc, glm::mat4& projMat, GLint& projLoc, GLint& timeLoc, const float& time, const GLsizei& element_count, const GLboolean& transposeMat) {
		glUniformMatrix4fv(vLoc, element_count, transposeMat, glm::value_ptr(viewMat));
		glUniformMatrix4fv(mLoc, element_count, transposeMat, glm::value_ptr(modelMat));
		glUniformMatrix4fv(projLoc, element_count, transposeMat, glm::value_ptr(projMat));
		glUniform1f(timeLoc, time);
	}

	void SetupCamera(glm::vec3& cam, const float& x, const float& y, const float& z) {
		cam = glm::vec3(x, y, z);
	}

	void SetupModelInCameraSpace(glm::vec3& modelpos, const float& u, const float& v, const float& w) {
		modelpos = glm::vec3(u, v, w);
	}

	// Trying to set the position in camera space for multiple objects... issues with implementation of variable arg list when encapsulated in other function calls
	void SetupModelsInCameraSpace(std::vector<glm::vec3> models, ...) {
		va_list args;				// A place to store the list of arguments
		va_start(args, models);		// Initializing arguments to store all values after models
		for (int i = 0; i < models.size(); ++i) {
			models[i] = va_arg(args, glm::vec3);
		}
		va_end(args);				// Cleans up the list
	}

	void Init(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, GLuint vao[], const GLsizei& numVAOs, GLuint vbo[], const GLsizei& numVBOs, float vertexArr[],
		const GLsizei& vArrLength, glm::mat4& projMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, const float& x, const float& y, const float& z, glm::vec3& modelpos, const float& u, const float& v, const float& w) {
		rProg = CreateShaderProgram(vp, fp);
		projMat = PerspectiveMat(window, rad, zNear, zFar);
		SetupCamera(cam, x, y, z);
		SetupModelInCameraSpace(modelpos, u, v, w);
		SetupVertexArr(vao, numVAOs, vbo, numVBOs, vertexArr, vArrLength, 0);
	}

	void Refresh(GLuint& program) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
	}

	void Display(GLFWwindow* window, GLuint& rProg, GLuint vbo[], const char* u_view, GLint& vLoc, glm::mat4& viewMat, const char* u_model, GLint& mLoc, glm::mat4& modelMat, const char* u_proj, GLint& projLoc, glm::mat4& projMat,
		const char* u_time, GLint& timeLoc, const float& currentTime, glm::vec3& cam, glm::vec3& modelpos, const GLsizei& polygon_count, const GLsizei& primitive_count)
	{
		Refresh(rProg);
		vLoc = glGetUniformLocation(rProg, u_view);
		mLoc = glGetUniformLocation(rProg, u_model);
		projLoc = glGetUniformLocation(rProg, u_proj);
		timeLoc = glGetUniformLocation(rProg, u_time);
		viewMat = ViewMat(cam);
		modelMat = ModelMat(modelpos);
		SetupUniformMat(viewMat, vLoc, modelMat, mLoc, projMat, projLoc, timeLoc, currentTime, 1, GL_FALSE);
		SetupBufferArr(vbo, 0, 3);
		Draw(GL_TRIANGLES, polygon_count, primitive_count);
	}

	void PreGameLoop(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[], float vertexArr[],
		const GLsizei& vArrLength, glm::mat4& projMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, const float& x, const float& y, const float& z, glm::vec3& modelpos, const float& u, const float& v, const float& w) {
		glfwSwapInterval(1);
		Init(window, rProg, vp, fp, vao, numVAOs, vbo, numVBOs, vertexArr, vArrLength, projMat, rad, zNear, zFar, cam, x, y, z, modelpos, u, v, w);
	}

	void GameLoop(GLFWwindow* window, GLuint& rProg, GLuint vbo[], const char* u_view, GLint& vLoc, glm::mat4& viewMat, const char* u_model, GLint& mLoc, glm::mat4& modelMat, const char* u_proj, GLint& projLoc,
		glm::mat4& projMat, const char* u_time, GLint& timeLoc, glm::vec3& cam, glm::vec3& modelpos, const GLsizei& polygon_count, const GLsizei& primitive_count)
	{
		while (!glfwWindowShouldClose(window)) {
			Display(window, rProg, vbo, u_view, vLoc, viewMat, u_model, mLoc, modelMat, u_proj, projLoc, projMat, u_time, timeLoc, glfwGetTime(), cam, modelpos, polygon_count, primitive_count);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}