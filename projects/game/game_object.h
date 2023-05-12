#pragma once
#include "transform.h"
#include "material.h"
#include "mesh.h"
#include "renderer.h"
#include <vector>
#include "file_reader.h"

namespace Game
{
	class Prefab final
	{
	public:
		Engine::Mesh* mesh;
		std::vector<Engine::Material*> materials;

		Prefab(Engine::Mesh* _mesh, const std::vector<Engine::Material*>& _materials);
		~Prefab();
	};

	class GameObject
	{
	private:
		Prefab* prefab;

		GameObject() = delete;

	public:
		Engine::Transform transform;
		bool cullable;

		GameObject(Prefab* _prefab);
		virtual ~GameObject();

		void Draw(Engine::Renderer& renderer);
	};
}