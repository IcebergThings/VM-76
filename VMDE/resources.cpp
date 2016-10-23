//=============================================================================
// ■ resources.cpp
//-----------------------------------------------------------------------------
//   处理资源的加载与释放等。
//=============================================================================

#include "global.hpp"

namespace Res {

	Texture* tex_unit[16];

	Texture::Texture(char* file, GLuint index) {
		glGenTextures(1, &this->texture);
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

		char* uniform_name = new char[16];
		sprintf(uniform_name, "colortex%d", index);
		log("Texture %s is loaded from %s", uniform_name, file);
		//glUniform1i(glGetUniformLocation(sh, (GLchar*) uniform_name), 0);
		xefree(uniform_name);

		unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		tex_unit[index] = this;
	}
}
