#pragma once
#include "window.h"
#include "renderer.h"
#include "game_object.h"
#include "text_renderer.h"
#include "player.h"
#include "point_system.h"

namespace Game
{
	class App
	{
	private:
		Engine::Window window;
		bool shouldClose;
		float deltaTime;

		struct KeyState
		{
			bool pressed;
			bool held;
			bool released;
		};

		std::unordered_map<int, KeyState> keys;

		Engine::Renderer renderer;
		TextRenderer textRenderer;
		Engine::Camera camera;

		std::unordered_map<std::string, Prefab*> prefabs;
		std::vector<GameObject*> gameObjects;

		GameObject* skybox;
		Player* player;
		GameObject* hunter;
		PointSystem pointSystem;

		std::vector<glm::vec2> mapData;
		std::vector<glm::vec2> obstacles;
		Engine::Transform cameraTransform;
		
		Engine::Transform startTextTransform;

		struct GameState
		{
			virtual void Enter(App* app) = 0;
			virtual void Update(App* app) = 0;
			virtual void DrawUI(App* app) = 0;
		};

		struct StartGame;
		struct PlayGame;
		struct GameOver;

		GameState* currentState;

		void ChangeState(GameState* newState);

	public:
		App();
		~App();

		bool Open();
		void UpdateLoop();
		void Close();
	};

	struct App::StartGame : public App::GameState
	{
		float startScreenTimer = 0.f;
		float startScreenDuration = 20.f;
		float textTimer = 0.f;
		int textIndex = 0;
		float textDurations[3] = {10.f, 5.f, 5.f};
		Engine::Transform textTransform;

		virtual void Enter(App* app) override;
		virtual void Update(App* app) override;
		virtual void DrawUI(App* app) override;
	};

	struct App::PlayGame : public App::GameState
	{
		float hunterFollowTimer = 0.f;
		float hunterFollowDuration = 3.f;
		float hunterSpawnSide = 1.f;
		Engine::Transform textTransform;

		virtual void Enter(App* app) override;
		virtual void Update(App* app) override;
		virtual void DrawUI(App* app) override;
	};

	struct App::GameOver : public App::GameState
	{
		float waitTimer = 0.f;
		float waitDuration = 4.f;
		Engine::Transform gameOverTextTransform;

		virtual void Enter(App* app) override;
		virtual void Update(App* app) override;
		virtual void DrawUI(App* app) override;
	};
}