#version 330 core
#pragma optimize(on)

out vec4 color;

in vec2 texcoord;

uniform sampler2D composite;

void main() {
	color = vec4(texture(composite, texcoord).rgb, 1.0);
}
