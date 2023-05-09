#pragma once
#include "../exts/glm/vec3.hpp"
#include "../exts/glm/gtc/quaternion.hpp"
#include "../exts/glm/matrix.hpp"

namespace Engine
{
	struct Transform
	{
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		glm::mat4 CalcMatrix() const;
		glm::vec3 Right() const;
		glm::vec3 Up() const;
		glm::vec3 Forward() const;
	};
}