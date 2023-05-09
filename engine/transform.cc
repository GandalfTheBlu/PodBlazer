#include "transform.h"

namespace Engine
{
	glm::mat4 Transform::CalcMatrix() const
	{
		glm::vec3 x = rotation * glm::vec3(scale.x, 0.f, 0.f);
		glm::vec3 y = rotation * glm::vec3(0.f, scale.y, 0.f);
		glm::vec3 z = rotation * glm::vec3(0.f, 0.f, scale.z);
		glm::mat4 m;
		m[0] = glm::vec4(x, 0.f);
		m[1] = glm::vec4(y, 0.f);
		m[2] = glm::vec4(z, 0.f);
		m[3] = glm::vec4(position, 1.f);
		return m;
	}

	glm::vec3 Transform::Right() const
	{
		return rotation * glm::vec3(1.f, 0.f, 0.f);
	}

	glm::vec3 Transform::Up() const
	{
		return rotation * glm::vec3(0.f, 1.f, 0.f);
	}

	glm::vec3 Transform::Forward() const
	{
		return rotation * glm::vec3(0.f, 0.f, 1.f);
	}
}