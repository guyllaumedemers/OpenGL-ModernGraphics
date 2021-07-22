#version 430

layout(location = 0) in vec3 position;				// retrieve model position
layout(location = 1) in vec2 textCoord;				// retrieve texture coordinate

out vec2 tc;										// store the texture coordinate and send to the fragment shader to apply texturing

layout(binding = 0) uniform sampler2D samp;			// access texture unit in GLSL

uniform mat4 mvp_matrix;							// proj_matrix

void main(void) {

	gl_Position = mvp_matrix * vec4(position, 1.0);
	tc = textCoord;
}