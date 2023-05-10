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

		static WorldSettings& Instance();
	};
}