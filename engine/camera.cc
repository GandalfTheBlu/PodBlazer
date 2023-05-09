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

	glm::mat4 Camera::CalcVP(const Transform& transform) const
	{
		glm::mat4 V = glm::lookAt(transform.position, transform.position + transform.Forward(), transform.Up());
		glm::mat4 P = glm::perspective(fovy, aspect, near, far);
		return P * V;
	}
}