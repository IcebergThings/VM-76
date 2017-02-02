#version 330 core
#pragma optimize(on)

out vec4 color;

in Vertex {
	vec4 texcolor;
	vec2 texcoord;
};

uniform float brightness;
uniform sampler2D colortex0;

void main() {
	vec4 finalc = texture(colortex0, texcoord.st);
	color = vec4(finalc * texcolor * brightness);
}
