#include "world_settings.h"

namespace Game
{
	WorldSettings::WorldSettings() : 
		directionalLight(glm::vec3(0.f)),
		currentTime(0.f),
		exhaustPosition(glm::vec3(0.f))
	{}

	WorldSettings& WorldSettings::Instance()
	{
		static WorldSettings instance;
		return instance;
	}
}