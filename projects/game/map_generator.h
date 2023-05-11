#pragma once
#include "game_object.h"
#include "../exts/glm/vec2.hpp"

namespace Game
{
	bool LoadMapFile(const std::string& path, std::vector<glm::vec2>& outPoints);
	void GenerateRoad(const std::vector<glm::vec2>& mapData, Prefab*& outPrefab);
	void SpawnSideObjects(const std::vector<glm::vec2>& mapData, const std::vector<Prefab*>& prefabs, std::vector<GameObject*>& outGameObjects);
}