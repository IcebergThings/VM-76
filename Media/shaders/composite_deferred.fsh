#version 330 core
#pragma optimize(on)

layout (location = 3) out vec4 composite;

in vec2 texcoord;

uniform sampler2D albedo;
uniform sampler2D lighting;

void main() {
	vec3 A = pow(texture(albedo, texcoord).rgb, vec3(2.2f));
	composite = vec4(A, 1.0) * texture(lighting, texcoord);
}
