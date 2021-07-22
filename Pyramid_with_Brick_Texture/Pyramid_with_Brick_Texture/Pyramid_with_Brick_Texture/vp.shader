#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textCoord;

out vec2 tc;

layout(binding = 0) uniform sampler2D samp;

uniform mat4 mvp_matrix;

out vec4 varyingColor;

void main(void) {

	gl_Position = mvp_matrix * vec4(position, 1.0);
	tc = textCoord;
}