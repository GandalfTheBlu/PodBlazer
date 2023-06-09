#pragma once
#include "game_object.h"
#include "../exts/glm/vec2.hpp"
#include "resources.h"

namespace Game
{
	bool LoadMapFile(const std::string& path, std::vector<glm::vec2>& outPoints);
	std::shared_ptr<Engine::Mesh> GenerateRoadMesh(const std::vector<glm::vec2>& mapData);
	void SpawnSideObjects(const std::vector<glm::vec2>& mapData, const std::vector<Prefab*>& prefabs, std::vector<GameObject*>& outGameObjects, float roadCenterOffset);
	void GenerateObstaclePoints(const std::vector<glm::vec2>& mapData, std::vector<glm::vec2>& outObstacles);
}