#version 330 core

out vec4 color;

in vertex_in {
	vec4 texcolor;
	vec2 texcoord;
} Vertex;

uniform float brightness;
uniform sampler2D colortex0;

void main() {
	vec4 finalc = texture(colortex0, Vertex.texcoord.st);
	color = vec4(finalc * Vertex.texcolor * brightness);
}
