//=============================================================================
// ■ init.cpp
//-----------------------------------------------------------------------------
//   所有初始化相关的代码都被放置在这里。
//=============================================================================

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

int init_shaders() {
  const GLchar* basic_2D_vsh_src = temp_vertexShaderSource;

  basic_2D_vsh = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(basic_2D_vsh, 1, &basic_2D_vsh_src, NULL);
  glCompileShader(basic_2D_vsh);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(basic_2D_vsh, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(basic_2D_vsh, 512, NULL, infoLog);
    printf("%s[init_shaders]: VSH compilation failed:\n%s\n", DBG_HEAD, infoLog);

    return 301;
  }

  // Fragment shader
  const GLchar* basic_2D_fsh_src = temp_fragmentShaderSource;

  basic_2D_fsh = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(basic_2D_fsh, 1, &basic_2D_fsh_src, NULL);
  glCompileShader(basic_2D_fsh);
  glGetShaderiv(basic_2D_fsh, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(basic_2D_fsh, 512, NULL, infoLog);
    printf("%s[init_shaders]: FSH compilation failed:\n%s\n", DBG_HEAD, infoLog);

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
    printf("%s[init_shaders]: Shader linking failed:\n%s\n", DBG_HEAD, infoLog);
  }
  glDeleteShader(basic_2D_vsh);
  glDeleteShader(basic_2D_fsh);

  return 0;
}

void setup_viewport() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}

int init_engine(int w, int h) {
	printf("%s[init_engine]: ENGINE INIT\n", DBG_HEAD);

	// 初始化VMDE结构
	VMDE = (VMDE_t*) malloc(sizeof(VMDE));
	VMDE->States = (VMDE_State_t*) malloc(sizeof(VMDE->States));
	VMDE->States->freeze = false;
	VMDE->frame_count = 0;
	VMDE->fps = 0;


	// GLFW库初始化
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		printf("%s[init_engine]: GLFW Init Failed\n", DBG_HEAD);
    exit(1);
	}

	// OpenGL 向前&向后兼容，使用GL 3.2 Core Profile，窗口大小不可变
	// 指定版本后便无需再检查是否支持指定版本，因为GLFW会处理此问题
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(w, h, GAME_NAME, NULL, NULL);
	if (!window) {
		glfwTerminate();
    printf("%s[init_engine]: GLFW Window Creation Failed. Your computer need OpenGL 3.2\n", DBG_HEAD);
    exit(1);
	}

	// 设置当前窗口GL上下文
	glfwMakeContextCurrent(window);

	// 初始化GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("%s[init_engine]: GLEW Inition failed. Your computer need OpenGL 3.2\n", DBG_HEAD);
    exit(1);
	}

  setup_viewport();

  glGenBuffers(1, &VBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// 初始化着色器「OpenGL 3.2没有固定管线了，着色器是被钦定的」
  int rc = init_shaders();
  if (rc != 0)
    return rc;

	return 0;
}
