#include "shader.h"
#include "file_manager.h"
#include <stdio.h>

namespace Engine
{
	std::unordered_set<Shader*> Shader::loadedShaders;

	Shader::Shader() :
		program(0)
	{}

	Shader::~Shader(){}

	bool Shader::ReloadAll()
	{
		bool success = true;
		for (auto& shader : loadedShaders)
			success &= shader->Reload();

		return success;
	}

	bool Shader::Reload()
	{
		Deinit();
		return Init(path);
	}

	bool Shader::Init(const std::string& shaderPath)
	{
		if (loadedShaders.count(this) == 0)
			loadedShaders.insert(this);

		path = shaderPath;

		std::string vertPath = shaderPath;
		vertPath.append("_vert.glsl");

		std::string fragPath = shaderPath;
		fragPath.append("_frag.glsl");

		std::string vertText;
		std::string fragText;

		if (!ReadTextFile(vertPath, vertText) || !ReadTextFile(fragPath, fragText))
			return false;

		const char* vertText_C = vertText.c_str();
		const char* fragText_C = fragText.c_str();

		// create and compile vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint vertexShaderLength = static_cast<GLint>(vertText.size());
		glShaderSource(vertexShader, 1, &vertText_C, &vertexShaderLength);
		glCompileShader(vertexShader);

		GLint shaderLogSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* errorMessage = new GLchar[shaderLogSize];
			glGetShaderInfoLog(vertexShader, shaderLogSize, NULL, errorMessage);
			printf("[ERROR] failed to compile vertex shader:\n%s\n", errorMessage);
			delete[] errorMessage;
			return false;
		}

		// create and compile fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLint fragmentShaderLength = static_cast<GLint>(fragText.size());
		glShaderSource(fragmentShader, 1, &fragText_C, &fragmentShaderLength);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* errorMessage = new GLchar[shaderLogSize];
			glGetShaderInfoLog(fragmentShader, shaderLogSize, NULL, errorMessage);
			printf("[ERROR] failed to compile fragment shader:\n%s\n", errorMessage);
			delete[] errorMessage;
			return false;
		}

		// create and link program
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* errorMessage = new GLchar[shaderLogSize];
			glGetProgramInfoLog(program, shaderLogSize, NULL, errorMessage);
			printf("[ERROR] failed to link program:\n%s\n", errorMessage);
			delete[] errorMessage;
			return false;
		}

		// clean up
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return true;
	}

	void Shader::Deinit()
	{
		glDeleteProgram(program);
	}

	void Shader::Use()
	{
		glUseProgram(program);
	}

	void Shader::StopUsing()
	{
		glUseProgram(0);
	}

	void Shader::AddLocationIfNeeded(const std::string& name)
	{
		if (nameToLocation.find(name) == nameToLocation.end())
			nameToLocation[name] = glGetUniformLocation(program, name.c_str());
	}

	void Shader::SetFloat(const std::string& name, GLfloat value)
	{
		AddLocationIfNeeded(name);
		glUniform1f(nameToLocation[name], value);
	}

	void Shader::SetVec2(const std::string& name, const GLfloat* valuePtr)
	{
		AddLocationIfNeeded(name);
		glUniform2fv(nameToLocation[name], 1, valuePtr);
	}

	void Shader::SetVec3(const std::string& name, const GLfloat* valuePtr)
	{
		AddLocationIfNeeded(name);
		glUniform3fv(nameToLocation[name], 1, valuePtr);
	}

	void Shader::SetMat3(const std::string& name, const GLfloat* valuePtr)
	{
		AddLocationIfNeeded(name);
		glUniformMatrix3fv(this->nameToLocation[name], 1, GL_FALSE, valuePtr);
	}

	void Shader::SetMat4(const std::string& name, const GLfloat* valuePtr)
	{
		AddLocationIfNeeded(name);
		glUniformMatrix4fv(this->nameToLocation[name], 1, GL_FALSE, valuePtr);
	}
}