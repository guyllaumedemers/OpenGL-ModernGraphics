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

	// temp - can be parametrized better
	glm::mat4 ModelMat(const float& x, const float& y, const float& z, const float& transf, const float& rotf) {
		float time = glfwGetTime();
		glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(x * time) * transf, cos(y * time) * transf, sin(z * time) * transf));
		glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotf * time, glm::vec3(1.0f, 0.0f, 0.0f));
		rotMat = glm::rotate(rotMat, rotf * time, glm::vec3(0.0f, 1.0f, 0.0f));
		rotMat = glm::rotate(rotMat, rotf * time, glm::vec3(0.0f, 0.0f, 1.0f));
		return rotMat * transMat;
	}

	glm::mat4 StaticMvMat(glm::vec3& cam, glm::vec3& model) {
		return glm::translate(glm::mat4(1.0f), -cam) * glm::translate(glm::mat4(1.0f), model);
	}

	glm::mat4 DynamicMvMat(glm::vec3& cam, const float& x, const float& y, const float& z, const float& transf, const float& rotf) {
		return glm::translate(glm::mat4(1.0f), -cam) * ModelMat(x, y, z, transf, rotf);
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

	void SetupVertexArr(const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[], float vertexArr[], const int& vArrLength, const int& bIndex) {
		glGenVertexArrays(numVAOs, vao);
		glBindVertexArray(vao[bIndex]);
		glGenBuffers(numVBOs, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[bIndex]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vArrLength, vertexArr, GL_STATIC_DRAW);
	}

	void SetupBufferArr(GLuint vbo[], const int& bIndex, const int& size) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo[bIndex]);
		glVertexAttribPointer(bIndex, size, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(bIndex);
	}

	void Draw(const GLenum& mode, const GLsizei& count) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(mode, 0, count);
	}

	void SetupUniformMat(GLint& mvLoc, GLint& projLoc, const GLsizei& count, const GLboolean& transpose, glm::mat4& mvMat, glm::mat4& perpMat) {
		glUniformMatrix4fv(mvLoc, count, transpose, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, count, transpose, glm::value_ptr(perpMat));
	}

	// Set initial position const and use translation matrix when doing motion
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

	void Init(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[], float vertexArr[],
		const int& vArrLength, glm::vec3& cam, const float& x, const float& y, const float& z, glm::vec3& modelpos, const float& u, const float& v, const float& w) {
		rProg = CreateShaderProgram(vp, fp);
		SetupCamera(cam, x, y, z);
		SetupModelInCameraSpace(modelpos, u, v, w);
		SetupVertexArr(numVAOs, vao, numVBOs, vbo, vertexArr, vArrLength, 0);
	}

	void Refresh(GLuint& program) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
	}

	void StaticDisplay(GLFWwindow* window, const double& currentTime, const char* mv, GLint& mvLoc, const char* proj, GLint& projLoc, GLuint& rProg,
		glm::mat4& mvMat, glm::mat4& perpMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, glm::vec3& modelpos, GLuint vbo[], const GLsizei& count)
	{
		Refresh(rProg);
		mvLoc = glGetUniformLocation(rProg, mv);
		projLoc = glGetUniformLocation(rProg, proj);
		perpMat = PerspectiveMat(window, rad, zNear, zFar);
		mvMat = StaticMvMat(cam, modelpos);
		SetupUniformMat(mvLoc, projLoc, 1, GL_FALSE, mvMat, perpMat);
		SetupBufferArr(vbo, 0, 3);
		Draw(GL_TRIANGLES, count);
	}

	void DynamicDisplay(GLFWwindow* window, const double& currentTime, const char* mv, GLint& mvLoc, const char* proj, GLint& projLoc, GLuint& rProg,
		glm::mat4& mvMat, glm::mat4& perpMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, const float& x, const float& y, const float& z,
		const float& transf, const float& rotf, GLuint vbo[], const GLsizei& count)
	{
		Refresh(rProg);
		mvLoc = glGetUniformLocation(rProg, mv);
		projLoc = glGetUniformLocation(rProg, proj);
		perpMat = PerspectiveMat(window, rad, zNear, zFar);
		mvMat = DynamicMvMat(cam, x, y, z, transf, rotf);
		SetupUniformMat(mvLoc, projLoc, 1, GL_FALSE, mvMat, perpMat);
		SetupBufferArr(vbo, 0, 3);
		Draw(GL_TRIANGLES, count);
	}

	void PreGameLoop(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[], float vertexArr[],
		const int& vArrLength, glm::vec3& cam, const float& x, const float& y, const float& z, glm::vec3& modelpos, const float& u, const float& v, const float& w) {
		glfwSwapInterval(1);
		Init(window, rProg, vp, fp, numVAOs, vao, numVBOs, vbo, vertexArr, vArrLength, cam, x, y, z, modelpos, u, v, w);
	}

	// temp mod to test the model matrix that update position over time
	void GameLoop(GLFWwindow* window, const double& currentTime, const char* mv, GLint& mvLoc, const char* proj, GLint& projLoc, GLuint& rProg,
		glm::mat4& mvMat, glm::mat4& perpMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, glm::vec3& modelpos, GLuint vbo[], const GLsizei& count)
	{
		while (!glfwWindowShouldClose(window)) {
			DynamicDisplay(window, currentTime, mv, mvLoc, proj, projLoc, rProg, mvMat, perpMat, rad, zNear, zFar, cam, 0.35f, 0.52f, 0.7f, 2.0f, 1.75f, vbo, count);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}