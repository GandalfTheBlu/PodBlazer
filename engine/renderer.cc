#include "renderer.h"

namespace Engine
{
	Renderer::Renderer() :
		nextResourceId(0)
	{
		defaultMeshGenerators["default::screen_quad"] = [](Mesh* mesh)
		{
			mesh->ScreenQuad();
		};
	}
	Renderer::~Renderer()
	{
		for (auto& e : loadedShaders)
		{
			e.second->Deinit();
			delete e.second;
		}

		for (auto& e : loadedMeshes)
		{
			e.second->Deinit();
			delete e.second;
		}

		for (auto& e : loadedTextures)
		{
			e.second->Deinit();
			delete e.second;
		}
	}

	ResourceId Renderer::GetShader(const std::string& shaderPath)
	{
		if (pathToId.count(shaderPath) != 0)
			return pathToId[shaderPath];

		Shader* shader = new Shader();
		if (!shader->Init(shaderPath))
		{
			delete shader;
			return -1;
		}

		ResourceId nextId = nextResourceId;
		loadedShaders[nextId] = shader;
		pathToId[shaderPath] = nextId;
		nextResourceId++;
		return nextId;
	}

	ResourceId Renderer::GetMesh(const std::string& meshPath)
	{
		if (pathToId.count(meshPath) != 0)
			return pathToId[meshPath];
		
		Mesh* mesh = new Mesh();

		if (defaultMeshGenerators.count(meshPath) != 0)
		{
			defaultMeshGenerators[meshPath](mesh);
		}
		else if (!mesh->LoadObj(meshPath))
		{
			delete mesh;
			return -1;
		}

		ResourceId nextId = nextResourceId;
		loadedMeshes[nextId] = mesh;
		pathToId[meshPath] = nextId;
		nextResourceId++;
		return nextId;
	}

	ResourceId Renderer::GetTexture(const std::string& texturePath)
	{
		if (pathToId.count(texturePath) != 0)
			return pathToId[texturePath];

		Texture* texture = new Texture();

		if (!texture->Init(texturePath))
		{
			delete texture;
			return -1;
		}

		ResourceId nextId = nextResourceId;
		loadedTextures[nextId] = texture;
		pathToId[texturePath] = nextId;
		nextResourceId++;
		return nextId;
	}

	

	void Renderer::AddObject(std::shared_ptr<GraphicsObject> object)
	{
		graphicsObjects.push_back(object);
	}

	void Renderer::RemoveObject(std::shared_ptr<GraphicsObject> object)
	{
		int i = (int)graphicsObjects.size() - 1;
		for (; i >= 0 && graphicsObjects[i] != object; i--);

		if (i >= 0)
		{
			graphicsObjects.erase(graphicsObjects.begin() + i);
		}
	}

	void Renderer::Render()
	{
		for (auto& obj : graphicsObjects)
		{
			Mesh* mesh = loadedMeshes[obj->mesh];
			mesh->Bind();

			for (size_t i=0; i < obj->materials.size() && i < mesh->primitiveGroups.size(); i++)
			{
				Material& mat = obj->materials[i];
				Shader* shader = loadedShaders[mat.shader];
				shader->Use();

				Texture* texture = nullptr;
				if (mat.texture != -1)
				{
					texture = loadedTextures[mat.texture];
					texture->Bind(GL_TEXTURE0);
				}

				// TODO: set shader variables

				mesh->Draw(i);

				if (texture != nullptr)
					texture->Unbind(GL_TEXTURE0);

				shader->StopUsing();
			}

			mesh->Unbind();
		}
	}
}