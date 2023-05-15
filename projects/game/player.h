#pragma once
#include "game_object.h"
#include "../exts/glm/vec2.hpp"

namespace Game
{
	class Player : public GameObject {
	private:

	public:
		Player(Prefab* _prefab);
		virtual ~Player();

		glm::vec3 cameraOffset = glm::vec3(0, 2, -5);

		glm::vec3 velocityVector = glm::vec3(0, 0, 0);
		float friction = 1.f;
		float acceleration = 10.f;
		float startAcceleration = 10.f;

		float yRotation = 0;
		float angularAcceleration = 5.f;
		float angularFriction = 3.f;
		float yAngularVelocity = 0;

		GameObject* exhaust;

		void Update(float deltaTime);
		bool IsColliding(const std::vector<glm::vec2>& mapData, const std::vector<glm::vec2>& obstacles, float maxRoadCenterOffset, float obstacleRadius);
	};
}