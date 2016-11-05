#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 texc;
layout (location = 3) in mat4 Model;

uniform mat4 View;
uniform mat4 Projection;

out vec4 texcolor;
out vec2 texcoord;

void main() {
	vec4 v = vec4(position, 1.0);
	gl_Position = Projection * (View * (Model * v));
	texcolor = vertex_color;
	texcoord = texc;
}
