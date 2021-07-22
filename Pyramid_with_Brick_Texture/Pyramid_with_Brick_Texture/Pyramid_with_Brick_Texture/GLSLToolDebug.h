#pragma once
#include "Utilities.h"

namespace GLSLToolDebug {

	void PrintShaderLog(GLuint shader) {
		int len = 0, chWrittn = 0;
		char* log = nullptr;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);						// return in parameter the number of character in the information log, including null termination
		if (len > 0) {
			log = (char*)malloc(len);											// Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
			glGetShaderInfoLog(shader, len, &chWrittn, log);
			std::printf("Shader info Log : %s", log);
			free(log);															// A block of memory previously allocated by a call to malloc, calloc or realloc is deallocated
		}
		return;
	}

	void PrintProgramLog(int prog) {
		int len = 0, chWrittn = 0;
		char* log = nullptr;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		if (len > 0) {
			log = (char*)malloc(len);
			glGetProgramInfoLog(prog, len, &chWrittn, log);
			std::printf("Program info Log : %s", log);
			free(log);
		}
		return;
	}

	bool CheckOpenGLError() {
		bool foundError = false;
		int glErr = glGetError();
		while (glErr != GL_NO_ERROR) {
			std::printf("glError : %d", glErr);									// output all error code to the console
			foundError = true;
			glErr = glGetError();
		}
		return foundError;
	}
}

