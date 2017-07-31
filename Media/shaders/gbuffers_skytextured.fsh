#version 330 core
#pragma optimize(on)

out vec4 color;

in Vertex {
	vec3 wpos;
};

uniform float brightness;
uniform samplerCube cubemap0;

void main() {
	color = vec4(texture(cubemap0, normalize(wpos)).rgb, 1.0);
}
