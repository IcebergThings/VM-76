//=============================================================================
// ■ resources.cpp
//-----------------------------------------------------------------------------
//   处理资源的加载与释放等。
//=============================================================================

#include "global.hpp"

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

	unsigned char* Texture::loadpng_bytes(const char* file, GLint* w, GLint* h) {
		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, file);

		// Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
		size_t u2 = 1; while(u2 < width) u2 *= 2;
		size_t v2 = 1; while(v2 < height) v2 *= 2;
		// Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
		double u3 = (double)width / u2;
		double v3 = (double)height / v2;

		// Make power of two version of the image.
		size_t buffer_size = sizeof(unsigned char*) * u2 * v2 * 4;
		unsigned char* image_raw = (unsigned char*) malloc(buffer_size);
		for(size_t y = 0; y < height; y++)
			for(size_t x = 0; x < width; x++)
				for(size_t c = 0; c < 4; c++)
					image_raw[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];

		image.clear();

		*w = width;
		*h = height;

		return image_raw;
	}

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

		unsigned char* image = loadpng_bytes(file, &width, &height);
		glTexImage2D(GL_TEXTURE_2D, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		if (parameter->MIPMAP_LEVEL > 0) glGenerateMipmap(GL_TEXTURE_2D);
		free(image);

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

		unsigned char* image = loadpng_bytes(files.A, &width, &height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		free(image);

		image = loadpng_bytes(files.B, &width, &height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		free(image);

		image = loadpng_bytes(files.C, &width, &height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		free(image);

		image = loadpng_bytes(files.D, &width, &height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		free(image);

		image = loadpng_bytes(files.E, &width, &height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		free(image);

		image = loadpng_bytes(files.F, &width, &height);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, parameter->PixelFormat, width, height, 0, parameter->PixelType, GL_UNSIGNED_BYTE, image);
		free(image);

		if (parameter->MIPMAP_LEVEL > 0) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		VMSC::ChangeTextureCubeMap(0);

		log("Cube Map is loaded to texture unit");
	}
}
