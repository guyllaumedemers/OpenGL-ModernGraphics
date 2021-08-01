#version 430

in vec2 tc;

out vec4 color;

uniform mat4 mvp_matrix;

layout(binding = 0) uniform sampler2D samp;		// receive the texture information that the texture unit 0 was assigned to

void main(void) {
	color = texture(samp, tc);
}