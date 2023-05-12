#include "game_object.h"
#include "world_settings.h"

namespace Game
{
	Prefab::Prefab(Engine::Mesh* _mesh, const std::vector<Engine::Material*>& _materials) :
		mesh(_mesh),
		materials(_materials)
	{}

	Prefab::~Prefab(){}

	GameObject::GameObject(Prefab* _prefab) :
		prefab(_prefab),
		cullable(true)
	{}

	GameObject::~GameObject(){}

	void GameObject::Draw(Engine::Renderer& renderer)
	{
		renderer.PushDrawCall(prefab->mesh, prefab->materials, transform);
	}
}