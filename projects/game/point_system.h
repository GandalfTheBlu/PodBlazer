#pragma once
#include "texture.h"
#include "shader.h"
#include "../exts/glm/vec2.hpp"
#include "../exts/glm/vec3.hpp"
#include <vector>

class PointSystem {

private:
	int currentCheckPoint = 0;
	int startCheckpoint = 0;

	std::vector<float> distances;

public:

	int currentPoints = -1;
	int highScore = 0;
	int checkpointAmount = 0;
	int lapsCompleted = 0;
	PointSystem();
	void PassCheckpoint(std::vector<glm::vec2> mapData, glm::vec3 playerPos);
	void SetStartPoint(std::vector<glm::vec2> mapData, glm::vec3 playerPos);
	void LoadHighScore();
	void SaveHighScore();
	void ResetScore();
};