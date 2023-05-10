#pragma once
#include "transform.h"

namespace Engine
{
	class Camera
	{
	private:
		float fovy, aspect, near, far;
	public:
		Camera();
		~Camera();

		void Init(float _fovy, float _aspect, float _near, float _far);

		glm::mat4 CalcV(const Transform& transform) const;
		glm::mat4 CalcP() const;
	};
}