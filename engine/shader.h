#pragma once
#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Engine
{
	class Shader
	{
	private:
		static std::unordered_set<Shader*> loadedShaders;

		std::string path;
		GLuint program;
		std::unordered_map<std::string, GLint> nameToLocation;

		void AddLocationIfNeeded(const std::string& name);

	public:
		Shader();
		~Shader();

		static bool ReloadAll();

		bool Reload();
		bool Init(const std::string& shaderPath);
		void Deinit();

		void Use();
		void StopUsing();

		void SetFloat(const std::string& name, GLfloat value);
		void SetVec3(const std::string& name, const GLfloat* valuePtr);
		void SetMat3(const std::string& name, const GLfloat* valuePtr);
		void SetMat4(const std::string& name, const GLfloat* valuePtr);
	};
}