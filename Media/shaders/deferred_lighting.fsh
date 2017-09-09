layout (location = 2) out vec4 light;

in vec2 texcoord;

uniform sampler2D normal;
uniform vec3 sunVec;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main() {
	vec3 N = texture(normal, texcoord).rgb * 2.0 - 1.0;
	float shade = max(dot(sunVec, N), 0.0);

	light = vec4(shade * lightColor + ambientColor, 1.0);
}
