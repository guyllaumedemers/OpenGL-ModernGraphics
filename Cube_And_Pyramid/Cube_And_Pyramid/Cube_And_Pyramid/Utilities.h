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
	glm::mat4 CreatePerspectiveMatrix(GLFWwindow* window, const float& rad, const float& zNear, const float& zFar) {
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		return glm::perspective(rad, (float)width / (float)height, zNear, zFar);
	}

	glm::mat4 CreateModelViewMatrix(glm::vec3& cam, glm::vec3& model) {
		return glm::translate(glm::mat4(1.0f), -cam) * glm::translate(glm::mat4(1.0f), model);
	}

	glm::mat4 CreateViewMatrix(glm::vec3& cam) {
		return glm::translate(glm::mat4(1.0f), -cam);
	}

	glm::mat4 CreateModelMatrix(glm::vec3& model) {
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

	void SetupCamera(glm::vec3& cam, const float& x, const float& y, const float& z) {
		cam = glm::vec3(x, y, z);
	}

	// Trying to set the position in camera space for multiple objects... issues with implementation of variable arg list when encapsulated in other function calls
	void SetupModelArrInCameraSpace(std::vector<glm::vec3> models, ...) {
		va_list args;				// A place to store the list of arguments
		va_start(args, models);		// Initializing arguments to store all values after models
		for (int i = 0; i < models.size(); ++i) {
			models[i] = va_arg(args, glm::vec3);
		}
		va_end(args);				// Cleans up the list
	}

	void Init(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, GLuint vao[], const GLsizei& numVAOs, GLuint vbo[], const GLsizei& numVBOs, void (*SetupVertices)(), glm::mat4& vMat, glm::mat4& projMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, const float& x, const float& y, const float& z, void (*SetupModelsInCameraSpace)(std::vector<glm::vec3>, ...), std::vector<glm::vec3> modelsArr) {
		rProg = CreateShaderProgram(vp, fp);
		projMat = CreatePerspectiveMatrix(window, rad, zNear, zFar);
		SetupCamera(cam, x, y, z);
		(*SetupModelsInCameraSpace)(modelsArr, glm::vec3{ 0.0f,-2.0f ,0.0f }, glm::vec3{ 4.0f,4.0f ,0.0f });		// sketchy since im doing it by hand
		(*SetupVertices)();
	}

	void SetVM_Mat(GLuint& rProg, glm::mat4& projMat, glm::vec3& cam, glm::vec3& modelpos, const GLsizei& modelVerticesLength, GLint& mvLoc, GLint& projLoc, const GLuint vbo[], const int& index) {
		glm::mat4 mvMat = CreateModelViewMatrix(cam, modelpos);

		glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projMat));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[index]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, modelVerticesLength);
	}

	void Refresh(GLuint& program) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
	}

	void Display(GLFWwindow* window, GLuint& rProg, GLuint vbo[], std::vector<glm::vec3> modelArrPos, std::vector<GLsizei> modelLengthArray, const char* u_mvMatrix,
		GLint& mvLoc, const char* u_proj, GLint& projLoc, glm::mat4& projMat, glm::vec3& cam)
	{
		Refresh(rProg);
		mvLoc = glGetUniformLocation(rProg, u_mvMatrix);
		projLoc = glGetUniformLocation(rProg, u_proj);
		try {
			for (int i = 0; i < modelArrPos.size(); ++i) {
				SetVM_Mat(rProg, projMat, cam, modelArrPos[i], modelLengthArray[i], mvLoc, projLoc, vbo, i);
			}
		}
		catch (const std::out_of_range& oor) {
			std::cerr << "Out of Range error: " << oor.what() << '\n';
			exit(EXIT_FAILURE);
		}
	}

	void PreGameLoop(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[], void (*SetupVertices)(),
		glm::mat4& vMat, glm::mat4& projMat, const float& rad, const float& zNear, const float& zFar, glm::vec3& cam, const float& x, const float& y, const float& z, void (*SetupModelsInCameraSpace)(std::vector<glm::vec3>, ...), std::vector<glm::vec3> modelsArr) {
		glfwSwapInterval(1);
		Init(window, rProg, vp, fp, vao, numVAOs, vbo, numVBOs, SetupVertices, vMat, projMat, rad, zNear, zFar, cam, x, y, z, SetupModelsInCameraSpace, modelsArr);
	}

	void GameLoop(GLFWwindow* window, GLuint& rProg, GLuint vbo[], std::vector<glm::vec3> modelArrPos, std::vector<GLsizei> modelLengthArray,
		const char* u_mvMatrix, GLint& mvLoc, const char* u_proj, GLint& projLoc, glm::mat4& projMat, glm::vec3& cam)
	{
		while (!glfwWindowShouldClose(window)) {
			Display(window, rProg, vbo, modelArrPos, modelLengthArray, u_mvMatrix, mvLoc, u_proj, projLoc, projMat, cam);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}