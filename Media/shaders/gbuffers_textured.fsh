layout (location = 0) out vec4 albedo;
layout (location = 1) out vec4 normal;

in Vertex {
	vec4 texcolor;
	vec2 texcoord;
	vec3 vpos;
	vec3 n;
};

uniform sampler2D colortex0;

void main() {
	vec4 finalc = texture(colortex0, texcoord.st);
	if (finalc.a < 0.5) discard;

	albedo = vec4(finalc * texcolor);

	normal = vec4(n * 0.5 + 0.5, 1.0);
}
