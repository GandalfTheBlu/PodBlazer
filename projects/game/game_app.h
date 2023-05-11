#pragma once
#include "window.h"
#include "renderer.h"
#include "game_object.h"
#include "../exts/glm/vec2.hpp"

namespace Game
{
	class App
	{
	private:
		Engine::Window window;
		bool shouldClose;

		Engine::Renderer renderer;
		Engine::Camera camera;

		Engine::Shader skyboxShader;

		std::unordered_map<std::string, Prefab*> prefabs;
		std::vector<GameObject*> gameObjects;

		std::vector<glm::vec2> mapData;

	public:
		App();
		~App();

		bool Open();
		void UpdateLoop();
		void Close();
	};
}