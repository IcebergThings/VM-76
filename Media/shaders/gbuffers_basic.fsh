#version 330 core
#pragma optimize(on)

layout (location = 3) out vec4 composite;

in vec4 texcolor;

void main() {
	composite = texcolor;
}
