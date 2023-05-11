#pragma once
#include "transform.h"
#include "material.h"
#include "mesh.h"
#include "renderer.h"
#include <unordered_map>
#include "file_reader.h"

namespace Game
{
	class Prefab
	{
	private:
		static Engine::Shader phongShader;

		Prefab() = delete;

	public:
		Engine::Mesh* mesh;
		std::vector<Engine::Material> materials;

		Prefab(const std::string& meshPath);
		Prefab(Engine::Mesh* newMesh, const std::vector<Engine::ObjMaterialInfo>& materialInfos);
		~Prefab();

		static bool Init(const std::string& phongShaderPath);
		static void Deinit();
	};

	class GameObject
	{
	private:
		Prefab* prefab;

		GameObject() = delete;

	public:
		Engine::Transform transform;
		bool cull;

		GameObject(Prefab* _prefab);
		virtual ~GameObject();

		void Draw(Engine::Renderer& renderer);
	};
}