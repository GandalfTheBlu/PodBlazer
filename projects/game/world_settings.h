#pragma once
#include "../exts/glm/vec3.hpp"

namespace Game
{
	class WorldSettings
	{
	private:
		WorldSettings();
		
	public:
		glm::vec3 directionalLight;
		float currentTime;
		glm::vec3 exhaustPosition;

		static WorldSettings& Instance();
	};
}