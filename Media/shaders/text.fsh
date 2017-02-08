#version 330 core
#pragma optimize(on)

out vec4 color;

in Vertex {
	vec4 texcolor;
	vec2 texcoord;
};

uniform sampler2D fontmap;

void main() {
	vec4 finalc = texture(fontmap, texcoord.st);
	color = vec4(texcolor.rgb, texcolor.a * finalc.r);
}
