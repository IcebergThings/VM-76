#version 330 core
#pragma optimize(on)

out vec4 color;

uniform sampler2D colortex;

in vec2 texcoord;

void main() {
	color = texture(colortex, texcoord);
}
