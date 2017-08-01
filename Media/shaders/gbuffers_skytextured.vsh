#version 330 core
#pragma optimize(on)

layout (location = 0) in vec3 position;
layout (location = 1) in mat4 Model;

layout (std140) uniform Matrices {
	mat4 Projection;
	mat4 View;
	mat4 ProjectionView;
} matrices;

out vec3 wpos;

void main() {
	vec4 v = vec4(position, 1.0);
	v = Model * v;

	wpos = v.xyz;
	gl_Position = matrices.ProjectionView * v;
}
