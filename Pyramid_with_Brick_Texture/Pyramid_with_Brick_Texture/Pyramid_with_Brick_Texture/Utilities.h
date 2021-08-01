#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SOIL2/SOIL2.h>
#include <fstream>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>
#include <stack>
#include "GLSLToolDebug.h"
#include "GLSLFileReader.h"

namespace Utilities {

	//MATRIX

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

	glm::mat4 RotateMatrix(const glm::vec3& rotation) {
		return glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), rotation);
	}

	glm::mat4 ScaleMatrix(const glm::vec3& scale) {
		return glm::scale(glm::mat4(1.0f), scale);
	}

	//SHADER

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
			std::puts("vertex compilation failed");
			GLSLToolDebug::PrintShaderLog(vShader);
		}

		glCompileShader(fShader);
		GLSLToolDebug::CheckOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
		if (fragCompiled != 1) {
			std::puts("fragment compilation failed");
			GLSLToolDebug::PrintShaderLog(fShader);
		}

		GLuint vfProgram = glCreateProgram();

		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);

		glLinkProgram(vfProgram);
		GLSLToolDebug::CheckOpenGLError();
		glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
		if (linked != 1) {
			std::puts("linking failed");
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

	//TEXTURE

	GLuint TextureLoading(const char* imgPath) {
		GLuint textureID;
		textureID = SOIL_load_OGL_texture(imgPath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		if (textureID == 0) {
			std::printf("could not find texture file : %s", imgPath);
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}

	//PROGRAM

	GLFWwindow* CreateWindow(const int& width, const int& height, const char* name, const int& major_version, const int& minor_version) {
		if (!glfwInit()) {
			std::puts("GLFW lib not initialize");
			exit(EXIT_FAILURE);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);

		return glfwCreateWindow(width, height, name, NULL, NULL);
	}

	void SetupVertices(std::vector<std::vector<float>> m_coord, std::vector<std::vector<float>> text_coord, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs,
		GLuint vbo[]) {
		glGenVertexArrays(numVAOs, vao);
		glBindVertexArray(vao[0]);
		glGenBuffers(numVBOs, vbo);

		for (int i = 0; i < m_coord.size(); ++i) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_coord[i].size(), &(*m_coord[i].begin()), GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * text_coord[i].size(), &(*text_coord[i].begin()), GL_STATIC_DRAW);
		}
	}

	void Draw(const GLenum& type, const GLint& first, const GLint& polygons_count, const GLenum& cullingType) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glFrontFace(cullingType);
		glDrawArrays(type, first, polygons_count);
	}

	void Refresh(const GLuint& rProg) {
		glClear(GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(rProg);
	}

	void Init(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const char* tp, glm::mat4& proj_matrix, const float& rad, const float& zNear, const float& zFar,
		std::vector<std::vector<float>> m_coord, std::vector<std::vector<float>> text_coord, GLuint& textureID, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[]) {
		rProg = CreateShaderProgram(vp, fp);
		textureID = TextureLoading(tp);
		proj_matrix = CreatePerspectiveMatrix(window, rad, zNear, zFar);
		SetupVertices(m_coord, text_coord, numVAOs, vao, numVBOs, vbo);
	}

	void Display(GLuint& rProg, glm::mat4& proj_matrix, std::stack<glm::mat4>& mv_stack, const glm::vec3& cam, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale, GLint& mvpLoc, const char* mvp_uniform, GLuint vbo[],
		const GLenum& type, GLuint& textureID, const GLint& polygons_count, const GLenum& cullingType) {

		glEnable(GL_CULL_FACE);

		mv_stack.push(mv_stack.top());
		mv_stack.top() *= CreateModelMatrix(position);
		mv_stack.push(mv_stack.top());
		mv_stack.top() *= RotateMatrix(rotation);
		mv_stack.top() *= ScaleMatrix(scale);

		glm::mat4 mvp_matrix = proj_matrix * mv_stack.top();

		mvpLoc = glGetUniformLocation(rProg, mvp_uniform);
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp_matrix));

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);									// positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);									// textures coordinates
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		Draw(type, 0, polygons_count, cullingType);
		mv_stack.pop();
	}

	void window_reshape_callback(GLFWwindow* window, int new_width, int new_height) {
		float aspect = (float)new_height / (float)new_height;
		glViewport(0, 0, new_width, new_height);
		//Cannot allow callback upon resizing the window since GLFWwindowsizefun doesnt allow to pass the projection matrix as argument
	}

	//GAME LOOP

	void PreGameLoop(GLFWwindow* window, GLuint& rProg, const char* vp, const char* fp, const char* tp, glm::mat4& proj_matrix, const float& rad, const float& zNear, const float& zFar,
		std::vector<std::vector<float>> m_coord, std::vector<std::vector<float>> text_coord, GLuint& textureID, const GLsizei& numVAOs, GLuint vao[], const GLsizei& numVBOs, GLuint vbo[]) {
		//glfwSetWindowSizeCallback(window, window_reshape_callback);
		glfwSwapInterval(1);
		Init(window, rProg, vp, fp, tp, proj_matrix, rad, zNear, zFar, m_coord, text_coord, textureID, numVAOs, vao, numVBOs, vbo);
	}

	void GameLoop(GLFWwindow* window, GLuint& rProg, glm::mat4& proj_matrix, std::stack<glm::mat4>& mv_stack, const glm::vec3& cam, std::vector<glm::vec3> positions, std::vector<glm::vec3> rotations, std::vector<glm::vec3> scales, GLint& mvpLoc, const char* mvp_uniform, GLuint vbo[],
		std::vector<std::vector<float>> m_coord, const GLenum& type, GLuint& textureID)
	{
		while (!glfwWindowShouldClose(window)) {
			Refresh(rProg);
			mv_stack.push(CreateViewMatrix(cam));
			for (int i = 0; i < positions.size(); ++i) {
				//Can set the culling type depending on the position index in the array if < x do this else do that
				Display(rProg, proj_matrix, mv_stack, cam, positions[i], rotations[i], scales[i], mvpLoc, mvp_uniform, vbo, type, textureID, (m_coord[i].size() / 3), GL_CCW);
			}
			while (!mv_stack.empty()) mv_stack.pop();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void DestroyWindow(GLFWwindow* window) {
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
}