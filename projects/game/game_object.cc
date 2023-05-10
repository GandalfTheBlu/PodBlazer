#include "game_object.h"
#include "file_reader.h"
#include "world_settings.h"

namespace Game
{
	Engine::Shader Prefab::phongShader;

	Prefab::Prefab(const std::string& meshPath)
	{
		std::vector<Engine::ObjMaterialInfo> objMaterialInfos;
		mesh = new Engine::Mesh();

		if (!Engine::LoadOBJFile(*mesh, objMaterialInfos, meshPath))
			return;

		for (auto& objMat : objMaterialInfos)
		{
			glm::vec3 color = objMat.color;
			materials.push_back({
				&phongShader,
				[color](const Engine::MaterialBindInput& inp)
				{
					glm::mat3 N = glm::transpose(glm::inverse(glm::mat3(inp.M)));
					glm::vec3 camPos = glm::vec3(inp.camTransform[3]);

					// vertex shader
					inp.shader->SetMat4("u_MVP", &inp.MVP[0][0]);
					inp.shader->SetMat4("u_M", &inp.M[0][0]);
					inp.shader->SetMat3("u_N", &N[0][0]);

					// fragment shader
					inp.shader->SetVec3("u_color", &color[0]);
					inp.shader->SetVec3("u_camPos", &camPos[0]);
					inp.shader->SetVec3("u_lightDir", &WorldSettings::Instance().directionalLight[0]);
				},
				nullptr
			});
		}
	}

	Prefab::~Prefab()
	{
		delete mesh;
	}

	bool Prefab::Init(const std::string& phongShaderPath)
	{
		return phongShader.Init(phongShaderPath);
	}

	void Prefab::Deinit()
	{
		phongShader.Deinit();
	}

	GameObject::GameObject(Prefab* _prefab) :
		prefab(_prefab)
	{}

	GameObject::~GameObject(){}

	void GameObject::Draw(Engine::Renderer& renderer)
	{
		renderer.Draw(prefab->mesh, prefab->materials, transform);
	}
}