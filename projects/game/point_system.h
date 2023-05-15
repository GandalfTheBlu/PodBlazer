#pragma once
#include "texture.h"
#include "shader.h"
#include "../exts/glm/vec2.hpp"
#include "../exts/glm/vec3.hpp"

class PointSystem {

private:
	int currentCheckPoint = 0;

	//std::vector<Engine::Texture> UITextures;
	Engine::Texture UITextures[10];

	Engine::Shader UIShader;


public:

	int currentPoints = 0;
	int highScore = 0;
	int checkpointAmount = 0;
	PointSystem();
	void PassCheckpoint(std::vector<glm::vec2> mapData, glm::vec3 playerPos);
	void SetStartPoint(std::vector<glm::vec2> mapData, glm::vec3 playerPos);
};