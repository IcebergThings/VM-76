#version 330 core
#pragma optimize(on)

layout (location = 3) out vec4 color;

in vec3 wpos;

uniform float brightness;
uniform samplerCube cubemap0;

void main() {
	vec3 tex = pow(texture(cubemap0, normalize(wpos)).rgb, vec3(2.2f));
	color = vec4(tex, 1.0);
}
