//=============================================================================
// ■ shaders.cpp
//-----------------------------------------------------------------------------
//   VMDE中与着色器有关的全局变量和类都在这里。
//=============================================================================

#include "VMDE.hpp"

Shaders::Shaders() {
}

void Shaders::add_string(GLenum type, GLchar* source) {
	if (!source) error("source is null");

	char gl_330_string[] = "#version 330 core\n#pragma optimize(on)\n#define VMDE_GL330\n";
	char gl_430_string[] = "#version 430 core\n#pragma optimize(on)\n#define VMDE_GL330\n#define VMDE_GL430\n";
	char *sources[2] = {VMDE->gl_ver == GL_43 ? gl_430_string : gl_330_string, source};

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 2, sources, NULL);
	glCompileShader(shader);
	check_shader_compilation(shader, "shader compilation failed");
	shaders[shader_count] = shader;
	shader_count++;
	check_gl_error;
}

void Shaders::add_file(GLenum type, const char* filename) {
	log("adding shader file: %s", filename);
	char* source = Util::read_file(filename);
	add_string(type, source);
	free(source);
}

void Shaders::check_shader_compilation(GLuint shader, const char* msg) {
	GLint success;
	GLchar info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 512, NULL, info_log);
	if (!success) error("%s:\n%s", msg, info_log);
	if (info_log[0]) log("%s", info_log);
}

void Shaders::check_linkage(GLuint program, const char* msg) {
	GLint success;
	GLchar info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(program, 512, NULL, info_log);
	if (!success) error("%s:\n%s", msg, info_log);
	if (info_log[0]) log("%s", info_log);
}

void Shaders::link_program() {
	// Link shaders
	program = glCreateProgram();
	for (size_t i = 0; i < shader_count; i++) {
		glAttachShader(program, shaders[i]);
	}
	glLinkProgram(program);
	check_linkage(program, "shaders linking failed");
	for (size_t i = 0; i < shader_count; i++) {
		glDeleteShader(shaders[i]);
	}
	check_gl_error;

	set_uniform_block("Matrices", &UBO_matrix, 3 * sizeof(glm::mat4), GL_STREAM_DRAW);
}

void Shaders::use() {
	glUseProgram(program);
	check_gl_error;
}

void Shaders::ProjectionView(glm::mat4 projection, glm::mat4 view) {
	mat[0] = projection;
	mat[1] = view;
	mat[2] = projection * view;
	update_uniform_block(UBO_matrix, 3 * sizeof(glm::mat4), (GLuint*) mat);
}

void Shaders::set_float(const char* identifier, GLfloat value) {
	GLuint loc = glGetUniformLocation(program, identifier);
	glUniform1f(loc, value);
	check_gl_error;
}

void Shaders::set_vec2(const char* identifier, glm::vec2 value) {
	GLuint loc = glGetUniformLocation(program, identifier);
	glUniform2f(loc, value.r, value.g);
}

void Shaders::set_vec3(const char* identifier, glm::vec3 value) {
	GLuint loc = glGetUniformLocation(program, identifier);
	glUniform3f(loc, value.r, value.g, value.b);
}

void Shaders::set_vec4(const char* identifier, glm::vec4 value) {
	GLuint loc = glGetUniformLocation(program, identifier);
	glUniform4f(loc, value.r, value.g, value.b, value.a);
}

void Shaders::set_int(const char* identifier, GLint value) {
	GLuint loc = glGetUniformLocation(program, identifier);
	glUniform1i(loc, value);
	check_gl_error;
}

void Shaders::set_texture(const char* identifier, Res::Texture* tex, GLuint index) {
	glActiveTexture(GL_TEXTURE0 + index);
	VMSC::ChangeTexture2D(tex->texture);
	set_int(identifier, index);
}

void Shaders::set_texture_cube(const char* identifier, Res::CubeMap* tex, GLuint index) {
	glActiveTexture(GL_TEXTURE0 + index);
	VMSC::ChangeTextureCubeMap(tex->texture);
	set_int(identifier, index);
}

bool Shaders::set_uniform_block(const char* identifier, GLuint* UBO, size_t size, GLuint type) {
	GLint loc = glGetUniformBlockIndex(program, identifier);
	if (loc == -1) return false;
	glUniformBlockBinding(program, loc, 0);
	glGenBuffers(1, UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, *UBO);
	glBufferData(GL_UNIFORM_BUFFER, size, 0, type);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return true;
}

void Shaders::update_uniform_block(GLuint UBO, size_t size, GLuint* buf) {
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, buf);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, size);
}

Shaders::~Shaders() {
	glDeleteProgram(program);
}
