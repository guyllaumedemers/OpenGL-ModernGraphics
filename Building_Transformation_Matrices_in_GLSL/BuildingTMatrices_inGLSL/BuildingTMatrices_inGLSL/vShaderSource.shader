#version 430

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
	if (gl_VertexID == 0) {
		gl_Position = vec4(0.25, -0.25, 0, 1);
	}
	else if (gl_VertexID == 1) {
		gl_Position = vec4(-0.25, -0.25, 0, 1);
	}
	else {
		gl_Position = vec4(0.25, 0.25, 0, 1);
	}
	gl_Position = RotateZ(radians(90))* gl_Position;
}