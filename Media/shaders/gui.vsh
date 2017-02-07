#version 330 core
#pragma optimize(on)

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 texc;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 Model;

layout (std140) uniform Matrices {
	mat4 Projection;
	mat4 View; // Unused
	mat4 ProjectionView;
} matrices;

out vec4 texcolor;
out vec2 texcoord;

void main() {
	gl_Position = matrices.Projection * Model * vec4(position, 1.0);
	texcolor = vertex_color;
	texcoord = texc;
}
