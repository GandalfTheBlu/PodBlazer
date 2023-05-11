#pragma once
#include "game_object.h"

class Player : public Game::GameObject {
private:

public:
	Player(Game::Prefab* _prefab);
	glm::vec3 cameraOffset = glm::vec3(0, 2, -5);

	glm::vec3 velocityVector = glm::vec3(0, 0, 0);
	float friction = 0.9f;
	float acceleration = 10.f;
	float maxSpeed = 5.f;

};