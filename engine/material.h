#pragma once
#include "shader.h"
#include "../exts/glm/matrix.hpp"

namespace Engine
{
	struct MaterialInput
	{
		glm::mat4 camTransform;
		glm::mat4 M;
		glm::mat4 MVP;
	};

	class Material
	{
	public:
		Shader* shader;
		
		Material();
		virtual ~Material();

		virtual void Apply(const MaterialInput& input) = 0;
		virtual void CleanUp();
	};
}