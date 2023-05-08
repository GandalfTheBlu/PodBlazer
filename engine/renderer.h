#pragma once
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

namespace Engine
{
	typedef int ResourceId;

	struct Material
	{
		float color[3];
		float roughness;
		ResourceId texture;
		ResourceId shader;
	};

	struct GraphicsObject
	{
		ResourceId mesh;
		Transform transform;
		std::vector<Material> materials;
	};

	class Renderer
	{
	private:
		std::unordered_map<std::string, ResourceId> pathToId;
		std::unordered_map<std::string, void(*)(Mesh*)> defaultMeshGenerators;
		std::unordered_map<ResourceId, Shader*> loadedShaders;
		std::unordered_map<ResourceId, Mesh*> loadedMeshes;
		std::unordered_map<ResourceId, Texture*> loadedTextures;
		ResourceId nextResourceId;

		std::vector<std::shared_ptr<GraphicsObject>> graphicsObjects;

	public:
		Renderer();
		~Renderer();

		ResourceId GetShader(const std::string& shaderPath);
		ResourceId GetMesh(const std::string& meshPath);
		ResourceId GetTexture(const std::string& texturePath);

		void AddObject(std::shared_ptr<GraphicsObject> object);
		void RemoveObject(std::shared_ptr<GraphicsObject> object);
		void Render();
	};
}