//=============================================================================
// ■ resources.cpp
//-----------------------------------------------------------------------------
//   处理资源的加载与释放等。
//=============================================================================

#include "global.hpp"

namespace Res {
	Texture* tex_unit[16];

	Texture::Texture(const char* file, GLuint index) {
		if (tex_unit[index]) {
			log("Texture unit %d is occupied", index);
			exit(1);
		}

		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

		unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		log("Texture is loaded from %s to texture unit %d", file, index);

		this->index = index;
		tex_unit[index] = this;
	}

	Texture* Texture::dispose() {
		//glDisableTexture(GL_TEXTURE0 + this->index)；
		glDeleteTextures(1, &this->texture);

		tex_unit[this->index] = NULL;

		return this;
	}
}
