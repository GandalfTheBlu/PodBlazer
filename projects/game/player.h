#pragma once
#include "game_object.h"

class Player : public Game::GameObject {
private:

public:
	Player(Game::Prefab* _prefab);
	glm::vec3 cameraOffset = glm::vec3(0, 0.6f, -2);

	glm::vec3 velocityVector = glm::vec3(0, 0, 0);
	float friction = 0.01f;
	float acceleration = 0.15f;
	float maxSpeed = 10.f;

	float yRotation = 0;
	float angularAcceleration = 2.5f;
	float angularFriction = 0.04f;
	float yAngularVelocity = 0;

};