#version 430

in vec4 varyingColor;

out vec4 color;

uniform mat4 mvp_matrix;

void main(void) {
	color = varyingColor;
}