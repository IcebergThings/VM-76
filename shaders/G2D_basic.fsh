#version 330 core
out vec4 color;
in vec4 texcolor;
in vec2 texcoord;
uniform float brightness;
uniform sampler2D colortex0;

void main() {
	vec4 finalc = vec4(0.0);
	finalc += texture(colortex0, texcoord.st);
	color = vec4(finalc * texcolor * brightness);
}
