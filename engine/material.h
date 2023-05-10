#pragma once
#include "shader.h"
#include "../exts/glm/matrix.hpp"
#include <functional>

namespace Engine
{
	struct MaterialBindInput
	{
		Shader* shader;
		glm::mat4 camTransform;
		glm::mat4 M;
		glm::mat4 MVP;
	};

	typedef std::function<void(const MaterialBindInput&)> MaterialBindFunction;
	typedef std::function<void()> MaterialUnbindFunction;

	struct Material
	{
		Shader* shader;
		MaterialBindFunction Bind;
		MaterialUnbindFunction Unbind;
	};
}