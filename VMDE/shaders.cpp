//=============================================================================
// ■ shaders.cpp
//-----------------------------------------------------------------------------
//   VMDE中与着色器有关的全局变量和类都在这里。
//=============================================================================

#include "global.hpp"

Shaders::Shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr) {
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
		log("Shaders error");
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
		log("Shaders error");
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
		log("Shaders error");
	}
	glDeleteShader(this->basic_2D_vsh);
	glDeleteShader(this->basic_2D_fsh);

	GLint loc = glGetUniformBlockIndex(shaderProgram, "Matrices");
	glUniformBlockBinding(shaderProgram, loc, 0);

	glGenBuffers(1, &UBO_matrix);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_matrix);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), 0, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Shaders* _shaders_in_use = NULL;

void Shaders::use() {
	if (this == _shaders_in_use) return;
	_shaders_in_use = this;
	glUseProgram(this->shaderProgram);
}

void Shaders::ProjectionView(glm::mat4 projection, glm::mat4 view) {
	mat[0] = projection;
	mat[1] = view;
	mat[2] = projection * view;
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_matrix);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 3 * sizeof(glm::mat4), mat);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO_matrix, 0, 3 * sizeof(glm::mat4));
}

void Shaders::set_float(const char* identifier, GLfloat value) {
	GLuint loc = glGetUniformLocation(this->shaderProgram, identifier);
	glUniform1f(loc, value);
}

void Shaders::set_int(const char* identifier, GLint value) {
	GLuint loc = glGetUniformLocation(this->shaderProgram, identifier);
	glUniform1i(loc, value);
}

void Shaders::set_texture(const char* identifier, Res::Texture* tex, GLuint index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
	set_float(identifier, index);
}
