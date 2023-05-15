#include "player.h"
#include "world_settings.h"

namespace Game
{
	Player::Player(Game::Prefab* _prefab) :
		GameObject(_prefab),
		exhaust(nullptr)
	{}

	Player::~Player() {}

	void Player::Update()
	{
		exhaust->transform.position = transform.position - transform.Forward() * 1.2f + transform.Up() * 0.38f;
		exhaust->transform.rotation = transform.rotation;
		WorldSettings::Instance().exhaustPosition = exhaust->transform.position;

	}

	bool Player::IsColliding(const std::vector<glm::vec2>& mapData, const std::vector<glm::vec2>& obstacles, float maxRoadCenterOffset, float obstacleRadius)
	{
		// check obstacles
		for (size_t i = 0; i < obstacles.size(); i++)
		{
			glm::vec2 toPlayer = glm::vec2(transform.position.x, transform.position.z) - obstacles[i];
			float dist2 = glm::dot(toPlayer, toPlayer);
			if (dist2 < obstacleRadius * obstacleRadius)
				return true;
		}

		// check road segments
		for (size_t i = 0; i < mapData.size(); i++)
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
			if (offset2 < maxRoadCenterOffset * maxRoadCenterOffset)
				return false;
		}

		return true;
	}
}