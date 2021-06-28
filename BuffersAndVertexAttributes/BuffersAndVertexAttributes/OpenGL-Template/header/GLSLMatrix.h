#pragma once
#include <glm/glm.hpp>

namespace GLSLMatrix {

	// translation matrix
	glm::mat4 Translate(float x, float y, float z) {
		glm::mat4 trans = glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			x, y, z, 1.0
		);
		return trans;
	}

	// rotation matrix
	glm::mat4 RotateX(float rad) {
		glm::mat4 xRot = glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, cos(rad), -sin(rad), 0.0,
			0.0, sin(rad), cos(rad), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		return xRot;
	}

	glm::mat4 RotateY(float rad) {
		glm::mat4 yRot = glm::mat4(
			cos(rad), 0.0, sin(rad), 0.0,
			0.0, 1.0, 0.0, 0.0,
			-sin(rad), 0.0, cos(rad), 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		return yRot;
	}

	glm::mat4 RotateZ(float rad) {
		glm::mat4 zRot = glm::mat4(
			cos(rad), -sin(rad), 0.0, 0.0,
			sin(rad), cos(rad), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		return zRot;
	}

	// scale matrix
	glm::mat4 Scale(float x, float y, float z) {
		glm::mat4 scale = glm::mat4(
			x, 0.0, 0.0, 0.0,
			0.0, y, 0.0, 0.0,
			0.0, 0.0, z, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
		return scale;
	}
}