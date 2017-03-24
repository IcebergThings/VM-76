#version 330 core
#pragma optimize(on)

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 vv;
layout (location = 2) out vec4 nn;

in Vertex {
	vec4 texcolor;
	vec3 vpos;
	vec3 n;
};

void main() {
	color = texcolor;

	vv = vec4(vpos, 1.0);
	nn = vec4(n * 0.5 + 0.5, 1.0);
}
