#version 430

in vec4 varyingColor;

out vec4 color;

uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform float time;

void main(void) {
	color = varyingColor;
}