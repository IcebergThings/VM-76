//=============================================================================
// ■ shaders.cpp
//-----------------------------------------------------------------------------
//   VMDE中与着色器有关的全局变量和类都在这里。
//=============================================================================

#include "global.hpp"

Shaders::Shaders(const GLchar* vsh_src, const GLchar* fsh_src) {
	if (!vsh_src) error("vsh_src is null");
	if (!fsh_src) error("fsh_src is null");

	// Vertex shader
	basic_2D_vsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(basic_2D_vsh, 1, &vsh_src, NULL);
	glCompileShader(basic_2D_vsh);
	check_shader_compilation(basic_2D_vsh, "Vertex shader compilation failed");

	// Fragment shader
	basic_2D_fsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(basic_2D_fsh, 1, &fsh_src, NULL);
	glCompileShader(basic_2D_fsh);
	check_shader_compilation(basic_2D_fsh, "Fragment shader compilation failed");

	link_program();
}

Shaders* Shaders::CreateFromFile(const char* vsh_src, const char* fsh_src) {
	char* temp_vertexShaderSource = Util::read_file(vsh_src);
	char* temp_fragmentShaderSource = Util::read_file(fsh_src);
	Shaders* temp_shader = new Shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
	XE(free, temp_vertexShaderSource);
	XE(free, temp_fragmentShaderSource);

	return temp_shader;
}

void Shaders::check_shader_compilation(GLuint shader, const char* msg) {
	GLint success;
	GLchar info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	glGetShaderInfoLog(shader, 512, NULL, info_log);
	if (!success) error("%s:\n%s", msg, info_log);
}

void Shaders::check_linkage(GLuint program, const char* msg) {
	GLint success;
	GLchar info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	glGetProgramInfoLog(program, 512, NULL, info_log);
	if (!success) error("%s:\n%s", msg, info_log);
}

void Shaders::link_program() {
	// Link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, basic_2D_vsh);
	glAttachShader(shaderProgram, basic_2D_fsh);
	glLinkProgram(shaderProgram);
	check_linkage(shaderProgram, "shaders linking failed");
	glDeleteShader(basic_2D_vsh);
	glDeleteShader(basic_2D_fsh);

	GLint loc = glGetUniformBlockIndex(shaderProgram, "Matrices");
	glUniformBlockBinding(shaderProgram, loc, 0);

	glGenBuffers(1, &UBO_matrix);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_matrix);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), 0, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Shaders::use() {
	glUseProgram(shaderProgram);
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
	GLuint loc = glGetUniformLocation(shaderProgram, identifier);
	glUniform1f(loc, value);
}

void Shaders::set_int(const char* identifier, GLint value) {
	GLuint loc = glGetUniformLocation(shaderProgram, identifier);
	glUniform1i(loc, value);
}

void Shaders::set_texture(const char* identifier, Res::Texture* tex, GLuint index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, tex->texture);
	set_float(identifier, index);
}

Shaders::~Shaders() {
	glDeleteProgram(shaderProgram);
}
