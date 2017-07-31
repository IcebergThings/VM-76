#version 330 core
#pragma optimize(on)

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 vv;
layout (location = 2) out vec4 nn;

in Vertex {
	vec3 wpos;
};

uniform float brightness;
uniform samplerCube cubemap0;

void main() {
	color = vec4(texture(cubemap0, normalize(wpos)).rgb, 1.0);

	vv = vec4(1.0);
	nn = vec4(1.0);
}
