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

	glm::mat4 CreateModelViewMatrix(const glm::vec3& cam, const glm::vec3& model) {
		return glm::translate(glm::mat4(1.0f), -cam) * glm::translate(glm::mat4(1.0f), model);
	}

	glm::mat4 CreateViewMatrix(const glm::vec3& cam) {
		return glm::translate(glm::mat4(1.0f), -cam);
	}

	glm::mat4 CreateModelMatrix(const glm::vec3& model) {
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

	void SetupVertices(std::vector<std::vector<float>> models, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[]) {
		glGenVertexArrays(numVAOs, vao);
		glBindVertexArray(vao[0]);
		glGenBuffers(numVBOs, vbo);

		for (int i = 0; i < models.size(); ++i) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * models[i].size(), &(*models[i].begin()), GL_STATIC_DRAW);
		}
	}

	void Draw(const GLenum& type, const GLint& first, const GLint& polygons_count) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(type, first, polygons_count);
	}

	void Refresh(const GLuint& rProg) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(rProg);
	}

	void Init(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, glm::mat4& pv_matrix, const glm::vec3& cam, const float& rad, const float& zNear, const float& zFar,
		std::vector<std::vector<float>> models, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[]) {
		rProg = CreateShaderProgram(vp, fp);
		pv_matrix = CreatePerspectiveMatrix(window, rad, zNear, zFar) * CreateViewMatrix(cam);
		SetupVertices(models, numVAOs, vao, numVBOs, vbo);
	}

	void Display(GLuint& rProg, glm::mat4& pv_matrix, glm::vec3& model, GLint& mvpLoc, const char* mvp_uniform, GLuint vbo[], const int& index,
		const GLenum& type, const GLint& first, const GLint& polygons_count) {
		glm::mat4 mvp_matrix = pv_matrix * CreateModelMatrix(model);
		mvpLoc = glGetUniformLocation(rProg, mvp_uniform);
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[index]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		Draw(type, first, polygons_count);
	}

#if game
#endif

	void PreGameLoop(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, glm::mat4& pv_matrix, const glm::vec3& cam, const float& rad, const float& zNear, const float& zFar,
		std::vector<std::vector<float>> models, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[]) {
		glfwSwapInterval(1);
		Init(window, rProg, vp, fp, pv_matrix, cam, rad, zNear, zFar, models, numVAOs, vao, numVBOs, vbo);
	}

	void GameLoop(GLFWwindow* window, GLuint& rProg, glm::mat4& pv_matrix, std::vector<glm::vec3> model_positions, GLint& mvpLoc, const char* mvp_uniform, GLuint vbo[],
		std::vector<std::vector<float>> models, const GLenum& type)
	{
		while (!glfwWindowShouldClose(window)) {
			Refresh(rProg);
			for (int i = 0; i < model_positions.size(); ++i) {
				Display(rProg, pv_matrix, model_positions[i], mvpLoc, mvp_uniform, vbo, i, type, 0, (models[i].size() / 3));
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
}