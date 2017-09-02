#version 330 core
#pragma optimize(on)

out vec4 color;

in vec2 texcoord;

uniform sampler2D composite;

vec3 vignette(vec3 color) {
    float dist = distance(texcoord, vec2(0.5f));
    dist = dist * 1.7 - 0.65;
    dist = smoothstep(0.0, 1.0, dist);
    return color.rgb * (1.0 - dist);
}

void tonemap(inout vec3 color) {
	color *= 1.2;

	const float a = 2.51f;
	const float b = 0.03f;
	const float c = 2.43f;
	const float d = 0.59f;
	const float e = 0.14f;
	color = (color*(a*color+b))/(color*(c*color+d)+e);

	color = vignette(color);

	color = pow(color, vec3(1.0 / 2.2));
}

void main() {
	vec3 c = texture(composite, texcoord).rgb;

	tonemap(c);

	color = vec4(c, 1.0);
}
