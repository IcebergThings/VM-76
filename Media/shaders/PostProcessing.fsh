#version 330 core
#pragma optimize(on)

out vec4 color;

uniform sampler2D colortex;

in vec2 texcoord;

const float A = 1.0;   //brightness multiplier
const float B = 0.47;  //black level (lower means darker and more constrasted, higher make the image whiter and less constrasted)
const float C = 0.12;   //constrast level
const float D = 0.2;
const float E = 0.02;
const float F = 0.3;
const float W = 48.0;

#define Uncharted2Tonemap(x) (((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F)

void colorAdjust(inout vec3 c) {
	const float ExposureBias = 2.0f;
	vec3 curr = Uncharted2Tonemap(ExposureBias * c);

	vec3 whiteScale = 1.0f / Uncharted2Tonemap(vec3(W));
	vec3 color = curr * whiteScale;
}

void main() {
	vec3 c = texture(colortex, texcoord).rgb;
	colorAdjust(c);
	color = vec4(c, 1.0);
}
