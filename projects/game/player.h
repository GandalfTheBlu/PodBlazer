#pragma once
#include "game_object.h"
#include "../exts/glm/vec2.hpp"

class Player : public Game::GameObject {
private:

public:
	Player(Game::Prefab* _prefab);
	virtual ~Player();

	glm::vec3 cameraOffset = glm::vec3(0, 2, -5);

	glm::vec3 velocityVector = glm::vec3(0, 0, 0);
	float friction = 0.03f;
	float acceleration = 1.f;
	float maxSpeed = 50.f;


	bool IsColliding(const std::vector<glm::vec2>& mapData, float maxOffset);
};