#version 430

layout(location = 0) in vec3 position;

uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 proj_matrix;
uniform float time;

out vec4 varyingColor;

mat4 Translate(float x, float y, float z) {
	mat4 trans = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, y, z, 1.0
	);
	return trans;
}

mat4 RotateX(float rad) {
	mat4 xRot = mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, cos(rad), -sin(rad), 0.0,
		0.0, sin(rad), cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	return xRot;
}

mat4 RotateY(float rad) {
	mat4 yRot = mat4(
		cos(rad), 0.0, sin(rad), 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sin(rad), 0.0, cos(rad), 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	return yRot;
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

void main(void) {

	float i = gl_InstanceID + time;
	float a = sin(2.0 * i) * 8.0;
	float b = sin(3.0 * i) * 8.0;
	float c = sin(4.0 * i) * 8.0;

	mat4 localRotX = RotateX(1.04 * i);
	mat4 localRotY = RotateY(1.04 * i);
	mat4 localRotZ = RotateZ(1.04 * i);

	mat4 localT = Translate(a, b, c);

	mat4 model_Mat = model_matrix * localT * localRotX * localRotY * localRotZ;
	mat4 mvMat = view_matrix * model_Mat;

	gl_Position = proj_matrix * mvMat * vec4(position, 1.0);
	varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}