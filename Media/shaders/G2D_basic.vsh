#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 vertex_color;
layout (location = 2) in vec2 texc;
uniform mat4 MVP;
out vec4 texcolor;
out vec2 texcoord;

void main() {
	vec4 v = vec4(position, 1.0);
	gl_Position = MVP * v;
	texcolor = vertex_color;
	texcoord = texc;
}
