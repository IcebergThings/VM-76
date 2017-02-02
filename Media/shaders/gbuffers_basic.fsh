#version 330 core
#pragma optimize(on)

out vec4 color;

in vec4 texcolor;

void main() {
	color = texcolor;
}
