#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>

namespace Engine
{
	class Shader
	{
	private:
		GLuint program;
		std::unordered_map<std::string, GLint> nameToLocation;

	public:
		Shader();
		~Shader();

		bool Init(const std::string& shaderPath);
		void Deinit();

		void Use();
		void StopUsing();
	};
}