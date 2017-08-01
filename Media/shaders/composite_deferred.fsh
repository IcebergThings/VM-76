#version 330 core
#pragma optimize(on)

layout (location = 3) out vec4 composite;

in vec2 texcoord;

uniform sampler2D albedo;
uniform sampler2D lighting;
uniform sampler2D depthtex;

uniform vec2 clipping;
#define near clipping.x
#define far clipping.y

uniform vec3 fogColor;

float linearizeDepth(float depth) {
	return (2.0 * near) / (far + near - depth * (far - near));
}

void main() {
	float depth = texture(depthtex, texcoord).r;
	float ldepth = linearizeDepth(depth);

	vec3 A = pow(texture(albedo, texcoord).rgb, vec3(2.2f));

	A *= texture(lighting, texcoord).rgb;
	A = mix(A, fogColor, ldepth);

	composite = vec4(A, 1.0);
}
