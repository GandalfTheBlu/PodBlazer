#include "world_settings.h"

namespace Game
{
	WorldSettings::WorldSettings(){}

	WorldSettings& WorldSettings::Instance()
	{
		static WorldSettings instance;
		return instance;
	}
}