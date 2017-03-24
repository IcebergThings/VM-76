#version 330 core
#pragma optimize(on)

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 Model;

layout (std140) uniform Matrices {
	mat4 Projection;
	mat4 View;
	mat4 ProjectionView;
} matrices;

uniform float opaque;

out Vertex {
	vec4 texcolor;
	vec3 vpos;
	vec3 n;
};

void main() {
	vec4 v = vec4(position, 1.0);

	v = matrices.View * Model * v;
	vpos = v.xyz;

	gl_Position = matrices.Projection * v;
	texcolor = vec4(vertex_color.rgb, vertex_color.a * opaque);

	n = mat3(matrices.View) * mat3(Model) * normal;
}
