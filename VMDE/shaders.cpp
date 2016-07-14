#include "global.hpp"

const GLchar* temp_vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";
const GLchar* temp_fragmentShaderSource = "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\n\0";

int Shaders::init_shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr) {
	const GLchar* basic_2D_vsh_src = vsh_src_ptr;

	basic_2D_vsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(basic_2D_vsh, 1, &basic_2D_vsh_src, NULL);
	glCompileShader(basic_2D_vsh);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(basic_2D_vsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(basic_2D_vsh, 512, NULL, infoLog);
		log("VSH compilation failed:\n%s", infoLog);
		return 301;
	}

	// Fragment shader
	const GLchar* basic_2D_fsh_src = fsh_src_ptr;

	basic_2D_fsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(basic_2D_fsh, 1, &basic_2D_fsh_src, NULL);
	glCompileShader(basic_2D_fsh);
	glGetShaderiv(basic_2D_fsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(basic_2D_fsh, 512, NULL, infoLog);
		log("FSH compilation failed:\n%s", infoLog);
		return 302;
	}

	// Link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, basic_2D_vsh);
	glAttachShader(shaderProgram, basic_2D_fsh);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		log("shaders linking failed:\n%s", infoLog);
		return 303;
	}
	glDeleteShader(basic_2D_vsh);
	glDeleteShader(basic_2D_fsh);

	return 0;
}

Shaders* main_shader;
