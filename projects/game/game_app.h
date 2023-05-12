#pragma once
#include "window.h"
#include "renderer.h"
#include "game_object.h"
#include "../exts/glm/vec2.hpp"
#include "player.h"

namespace Game
{
	class App
	{
	private:
		Engine::Window window;
		bool shouldClose;

		Engine::Renderer renderer;
		Engine::Camera camera;

		std::unordered_map<std::string, Prefab*> prefabs;
		std::vector<GameObject*> gameObjects;

		GameObject* skybox;
		Player* player;

		std::vector<glm::vec2> mapData;

	public:
		App();
		~App();

		bool Open();
		void UpdateLoop();
		void Close();
	};
}