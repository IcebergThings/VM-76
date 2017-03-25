#version 330 core
#pragma optimize(on)

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 vv;
layout (location = 2) out vec4 nn;

in Vertex {
	vec4 texcolor;
	vec2 texcoord;
	vec3 vpos;
};

uniform float brightness;
uniform sampler2D colortex0;

void main() {
	vec4 finalc = texture(colortex0, texcoord.st);
	color = vec4(finalc * texcolor);

	vv = vec4(vpos, 1.0);
	nn = vec4(0.0, 0.0, 0.0, 1.0);
}
