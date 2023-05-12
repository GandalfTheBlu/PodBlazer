#include "player.h"

namespace Game
{
	Player::Player(Game::Prefab* _prefab) :
		GameObject(_prefab)
	{}

	Player::~Player() {}

	bool Player::IsColliding(const std::vector<glm::vec2>& mapData, float maxOffset)
	{
		for (int i = 0; i < (int)mapData.size(); i++)
		{
			glm::vec2 p0 = mapData[i];
			glm::vec2 p1 = mapData[(i + 1) % mapData.size()];

			glm::vec2 dir = p1 - p0;
			glm::vec2 p0ToPlayer = glm::vec2(transform.position.x, transform.position.z) - p0;
			float distAlongDir = glm::dot(p0ToPlayer, glm::normalize(dir));
			float dir2 = glm::dot(dir, dir);

			float margin = 2.f;

			if (distAlongDir < -margin || distAlongDir * distAlongDir > dir2)
				continue;

			float offset2 = glm::dot(p0ToPlayer, p0ToPlayer) - distAlongDir * distAlongDir;
			if (offset2 < maxOffset * maxOffset)
				return false;
		}

		return true;
	}
}