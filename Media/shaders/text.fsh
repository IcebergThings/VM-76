#version 330 core
#pragma optimize(on)

out vec4 color;

in Vertex {
	vec4 texcolor;
	vec2 texcoord;
};

uniform sampler2D fontmap;

void main() {
	vec3 finalc = texture(fontmap, texcoord.st).rgb;
	color = vec4(texcolor.rgb, texcolor.a * dot(finalc,vec3(0.2126, 0.7152, 0.0722)));
}
