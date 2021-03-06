#version 430

layout(location = 0) in vec3 position;

mat4 Scale(float x, float y, float z) {
	mat4 scale = mat4(
		x, 0.0, 0.0, 0.0,
		0.0, y, 0.0, 0.0,
		0.0, 0.0, z, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	return scale;
}

mat4 RotateZ(float rad) {
	mat4 zRot = mat4(
		cos(rad), -sin(rad), 0.0, 0.0,
		sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	return zRot;
}

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void) {
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);						// order matter
}