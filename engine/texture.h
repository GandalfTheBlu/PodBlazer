#pragma once
#include <GL/glew.h>
#include <string>

namespace Engine
{
	struct TextureFormat
	{
		bool flipVertical;
		GLenum format;

		TextureFormat();
	};

	class Texture
	{
	private:
		GLint width;
		GLint height;
		GLuint texture;

	public:
		Texture();
		~Texture();

		bool Init(const std::string& imagePath, const TextureFormat& format = TextureFormat());
		void Deinit();

		void Bind(GLuint binding) const;
		void Unbind(GLuint binding) const;
	};
}