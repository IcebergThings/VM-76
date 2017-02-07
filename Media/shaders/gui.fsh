#version 330 core
#pragma optimize(on)

out vec4 color;

uniform sampler2D atlastex;

in vec4 texcolor;
in vec2 texcoord;

void main() {
	color = texture(atlastex, texcoord) * texcolor;
}
