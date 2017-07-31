#version 330 core
#pragma optimize(on)

layout (location = 0) out vec4 albedo;
layout (location = 1) out vec4 normal;

in Vertex {
	vec4 texcolor;
	vec3 vpos;
	vec3 n;
};

void main() {
	albedo = texcolor;
	normal = vec4(n * 0.5 + 0.5, 1.0);
}
