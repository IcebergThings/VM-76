layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 texc;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 Model;

layout (std140) uniform Matrices {
	mat4 Projection;
	mat4 View;
	mat4 ProjectionView;
} matrices;

out Vertex {
	vec4 texcolor;
	vec2 texcoord;
	vec3 vpos;
	vec3 n;
};

void main() {
	vec4 v = vec4(position, 1.0);

	v = matrices.View * Model * v;
	vpos = v.xyz;

	gl_Position = matrices.Projection * v;
	texcolor = vertex_color;
	texcoord = texc;

	n = mat3(matrices.View) * mat3(Model) * normal;
}
