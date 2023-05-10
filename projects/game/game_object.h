#pragma once
#include "transform.h"
#include "material.h"
#include "mesh.h"
#include "renderer.h"
#include <unordered_map>

namespace Game
{
	class GameObjectInstance
	{
	private:
		static Engine::Shader phongShader;

		GameObjectInstance() = delete;

	public:
		Engine::Mesh* mesh;
		std::vector<Engine::Material> materials;

		GameObjectInstance(const std::string& meshPath);
		~GameObjectInstance();

		static bool Init(const std::string& phongShaderPath);
		static void Deinit();
	};

	class GameObject
	{
	private:
		GameObjectInstance* instance;

		GameObject() = delete;

	public:
		Engine::Transform transform;

		GameObject(GameObjectInstance* _instance);
		~GameObject();

		void Draw(Engine::Renderer& renderer);
	};
}