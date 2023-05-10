#pragma once
#include "window.h"
#include "renderer.h"
#include "game_object.h"

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

		std::unordered_map<std::string, GameObjectInstance*> prefabs;
		std::vector<GameObject*> gameObjects;

	public:
		App();
		~App();

		bool Open();
		void UpdateLoop();
		void Close();
	};
}