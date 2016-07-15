#version 330 core
out vec4 color;
uniform float brightness;
void main() {
	color = vec4(vec3(0.8f, 0.3f, 0.4f) * brightness, 1.0f);
}
