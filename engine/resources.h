#pragma once
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "material.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace Engine
{
	class Resources
	{
	private:
		std::unordered_map<std::string, std::shared_ptr<Mesh>> loadedMeshes;
		std::unordered_map<std::string, std::shared_ptr<Texture>> loadedTextures;
		std::unordered_map<std::string, std::shared_ptr<Shader>> loadedShaders;
		std::unordered_map<std::string, std::shared_ptr<Material>> loadedMaterials;

		Resources();

	public:
		static Resources& Instance();

		void CleanUp();

		void StoreMesh(const std::string& name, const std::shared_ptr<Mesh>& mesh);
		void StoreTexture(const std::string& name, const std::shared_ptr<Texture>& texture);
		void StoreShader(const std::string& name, const std::shared_ptr<Shader>& shader);
		void StoreMaterial(const std::string& name, const std::shared_ptr<Material>& material);

		Mesh* GetMesh(const std::string& name);
		Texture* GetTexture(const std::string& name);
		Shader* GetShader(const std::string& name);
		Material* GetMaterial(const std::string& name);
	};
}