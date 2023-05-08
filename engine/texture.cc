#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>

namespace Engine
{
	TextureFormat::TextureFormat() :
		flipVertical(true),
		format(GL_RGB)
	{}

	Texture::Texture() :
		width(-1),
		height(-1),
		texture(0)
	{}

	Texture::~Texture(){}

	bool Texture::Init(const std::string& imagePath, const TextureFormat& format)
	{
		stbi_set_flip_vertically_on_load(format.flipVertical);

		int loadFormat = 0;
		switch (format.format) 
		{
		case GL_RGBA: 
			loadFormat = 4;
			break;
		case GL_RGB: 
			loadFormat = 3;
			break;
		case GL_RG: 
			loadFormat = 2;
			break;
		case GL_R:
			loadFormat = 1;
			break;
		}

		if (loadFormat == 0)
		{
			printf("[ERROR] undefined texture format\n");
			return false;
		}

		int colorChannels;
		unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &colorChannels, loadFormat);

		if (data == nullptr)
		{
			printf("[ERROR] failed to load texture '%s'\n", imagePath.c_str());
			return false;
		}

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format.format, width, height, 0, format.format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);

		return true;
	}

	void Texture::Deinit()
	{
		glDeleteTextures(1, &texture);
	}

	void Texture::Bind(GLuint binding) const
	{
		glActiveTexture(binding);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void Texture::Unbind(GLuint binding) const
	{
		glActiveTexture(binding);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}