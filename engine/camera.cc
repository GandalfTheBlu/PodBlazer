#include "camera.h"

namespace Engine
{
	Camera::Camera() :
		fovy(0.f),
		aspect(0.f),
		near(0.f),
		far(0.f)
	{}

	Camera::~Camera(){}

	void Camera::Init(float _fovy, float _aspect, float _near, float _far)
	{
		fovy = _fovy;
		aspect = _aspect;
		near = _near;
		far = _far;
	}

	glm::mat4 Camera::CalcV(const Transform& transform) const
	{
		glm::vec3 z = glm::normalize(-transform.Forward());
		glm::vec3 x = glm::normalize(glm::cross(z, transform.Up()));
		glm::vec3 y = glm::cross(x, z);

		return glm::mat4(
			glm::vec4(x.x, y.x, z.x, 0.f),
			glm::vec4(x.y, y.y, z.y, 0.f),
			glm::vec4(x.z, y.z, z.z, 0.f),
			glm::vec4(-glm::dot(x, transform.position), -glm::dot(y, transform.position), -glm::dot(z, transform.position), 1.f)
		);
	}

	glm::mat4 Camera::CalcP() const
	{
		return glm::perspective(fovy, aspect, near, far);
	}
}