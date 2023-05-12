#include "resources.h"

namespace Engine
{
	Resources::Resources(){}

	Resources& Resources::Instance()
	{
		static Resources instance;
		return instance;
	}

	void Resources::CleanUp()
	{
		for (auto e : loadedMeshes)
			e.second->Deinit();

		for (auto e : loadedTextures)
			e.second->Deinit();

		for (auto e : loadedShaders)
			e.second->Deinit();

		loadedMeshes.clear();
		loadedTextures.clear();
		loadedShaders.clear();
		loadedMaterials.clear();
	}

	void Resources::StoreMesh(const std::string& name, const std::shared_ptr<Mesh>& mesh)
	{
		if (loadedMeshes.count(name) != 0)
		{
			printf("[WARNING] mesh with name '%s' already stored\n", name.c_str());
			return;
		}

		loadedMeshes[name] = mesh;
	}

	void Resources::StoreTexture(const std::string& name, const std::shared_ptr<Texture>& texture)
	{
		if (loadedTextures.count(name) != 0)
		{
			printf("[WARNING] texture with name '%s' already stored\n", name.c_str());
			return;
		}

		loadedTextures[name] = texture;
	}

	void Resources::StoreShader(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		if (loadedShaders.count(name) != 0)
		{
			printf("[WARNING] shader with name '%s' already stored\n", name.c_str());
			return;
		}

		loadedShaders[name] = shader;
	}

	void Resources::StoreMaterial(const std::string& name, const std::shared_ptr<Material>& material)
	{
		if (loadedMaterials.count(name) != 0)
		{
			printf("[WARNING] material with name '%s' already stored\n", name.c_str());
			return;
		}

		loadedMaterials[name] = material;
	}

	Mesh* Resources::GetMesh(const std::string& name)
	{
		if (loadedMeshes.count(name) == 0)
		{
			printf("[WARNING] mesh with name '%s' not found in storage\n", name.c_str());
			return nullptr;
		}

		return loadedMeshes[name].get();
	}

	Texture* Resources::GetTexture(const std::string& name)
	{
		if (loadedTextures.count(name) == 0)
		{
			printf("[WARNING] texture with name '%s' not found in storage\n", name.c_str());
			return nullptr;
		}

		return loadedTextures[name].get();
	}

	Shader* Resources::GetShader(const std::string& name)
	{
		if (loadedShaders.count(name) == 0)
		{
			printf("[WARNING] shader with name '%s' not found in storage\n", name.c_str());
			return nullptr;
		}

		return loadedShaders[name].get();
	}

	Material* Resources::GetMaterial(const std::string& name)
	{
		if (loadedMaterials.count(name) == 0)
		{
			printf("[WARNING] material with name '%s' not found in storage\n", name.c_str());
			return nullptr;
		}

		return loadedMaterials[name].get();
	}
}