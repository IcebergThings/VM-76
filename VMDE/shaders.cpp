//=============================================================================
// ■ shaders.cpp
//-----------------------------------------------------------------------------
//   VMDE中与着色器有关的全局变量和类都在这里。
//=============================================================================

#include "global.hpp"

const GLchar* temp_vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"layout (location = 1) in vec4 vertex_color;\n"
	"layout (location = 2) in vec2 texc;\n"
	"uniform mat4 viewMatrix;\n"
	"out vec4 texcolor;\n"
	"out vec2 texcoord;\n"
	"void main() {\n"
	"gl_Position = viewMatrix * vec4(position, 1.0);\n"
	"texcolor = vertex_color;\n"
	"texcoord = texc;\n"
	"}";
const GLchar* temp_fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 color;\n"
	"in vec4 texcolor;\n"
	"in vec2 texcoord;\n"
	"uniform float brightness;\n"
	"uniform sampler2D colortex0;\n"
	"void main() {\n"
	"color = vec4(texture(colortex0, texcoord) * texcolor * brightness);\n"
	"}";

void Shaders::init_shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr) {
	const GLchar* basic_2D_vsh_src = vsh_src_ptr;
	GLint success;
	GLchar info_log[512];

	this->basic_2D_vsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->basic_2D_vsh, 1, &basic_2D_vsh_src, NULL);
	glCompileShader(this->basic_2D_vsh);
	glGetShaderiv(this->basic_2D_vsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(this->basic_2D_vsh, 512, NULL, info_log);
		log("VSH compilation failed:\n%s", info_log);
		rb_raise(rb_eRuntimeError, "Shaders error");
	}

	// Fragment shader
	const GLchar* basic_2D_fsh_src = fsh_src_ptr;

	this->basic_2D_fsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->basic_2D_fsh, 1, &basic_2D_fsh_src, NULL);
	glCompileShader(this->basic_2D_fsh);
	glGetShaderiv(this->basic_2D_fsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(this->basic_2D_fsh, 512, NULL, info_log);
		log("FSH compilation failed:\n%s", info_log);
		rb_raise(rb_eRuntimeError, "Shaders error");
	}
}

void Shaders::link_program() {
	// Link shaders
	GLint success;
	GLchar info_log[512];

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, this->basic_2D_vsh);
	glAttachShader(shaderProgram, this->basic_2D_fsh);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
		log("shaders linking failed:\n%s", info_log);
		rb_raise(rb_eRuntimeError, "Shaders error");
	}
	glDeleteShader(this->basic_2D_vsh);
	glDeleteShader(this->basic_2D_fsh);
}

Shaders* main_shader;
