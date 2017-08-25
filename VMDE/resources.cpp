//=============================================================================
// ■ resources.cpp
//-----------------------------------------------------------------------------
//   处理资源的加载与释放等。
//=============================================================================

#include "VMDE.hpp"

namespace Res {

	struct TextureParameters DefaultTextureParameters = {
		GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT,
		GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST,
		3,
		GL_RGBA16, GL_RGBA
	};

	struct TextureParameters LinearTextureParameters = {
		GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT,
		GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR,
		3,
		GL_RGBA16, GL_RGBA
	};

	Texture::Texture() {}

	Texture::Texture(const char* file, struct TextureParameters* p) {
		parameter = p ? p : &DefaultTextureParameters;

		glGenTextures(1, &this->texture);
		VMSC::ChangeTexture2D(this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter->WRAP_MODE_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter->WRAP_MODE_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter->MIN_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter->MAG_FILTER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, parameter->MIPMAP_LEVEL);

		unsigned char* image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		if (parameter->MIPMAP_LEVEL > 0) glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
		VMSC::ChangeTexture2D(0);

		log("Texture is loaded from %s to texture unit", file);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &this->texture);
	}

	CubeMap::CubeMap(struct CubeMapFiles files, struct TextureParameters* p) {
		parameter = p ? p : &DefaultTextureParameters;

		glGenTextures(1, &this->texture);
		VMSC::ChangeTextureCubeMap(this->texture);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, parameter->WRAP_MODE_S);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, parameter->WRAP_MODE_T);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, parameter->MIN_FILTER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, parameter->MAG_FILTER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, parameter->MIPMAP_LEVEL);

		unsigned char* image = SOIL_load_image(files.A, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		image = SOIL_load_image(files.B, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		image = SOIL_load_image(files.C, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		image = SOIL_load_image(files.D, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		image = SOIL_load_image(files.E, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		image = SOIL_load_image(files.F, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		if (parameter->MIPMAP_LEVEL > 0) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		VMSC::ChangeTextureCubeMap(0);

		log("Cube Map is loaded to texture unit");
	}
}
