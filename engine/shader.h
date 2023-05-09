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

		void AddLocationIfNeeded(const std::string& name);

	public:
		Shader();
		~Shader();

		bool Init(const std::string& shaderPath);
		void Deinit();

		void Use();
		void StopUsing();

		void SetFloat(const std::string& name, GLfloat value);
		void SetVec3(const std::string& name, const GLfloat* valuePtr);
		void SetMat4(const std::string& name, const GLfloat* valuePtr);
	};
}