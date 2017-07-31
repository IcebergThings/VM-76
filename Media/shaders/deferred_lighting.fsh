#version 330 core
#pragma optimize(on)

layout (location = 2) out vec4 light;

in vec2 texcoord;

uniform sampler2D normal;
uniform vec3 sunVec;

void main() {
	vec3 N = texture(normal, texcoord).rgb * 2.0 - 1.0;
	float shade = max(dot(sunVec, N), 0.0) * 0.7 + 0.3;

	light = vec4(vec3(shade), 1.0);
}
