#include "renderer.h"

namespace Engine
{
	Renderer::Renderer() : 
		cameraTransform(glm::mat4()),
		VP(glm::mat4())
	{}

	Renderer::~Renderer(){}

	void Renderer::SetCamera(const Camera& camera, const Transform& _cameraTransform)
	{
		cameraTransform = _cameraTransform.CalcMatrix();
		VP = camera.CalcP() * camera.CalcV(_cameraTransform);
	}

	void Renderer::Draw(Mesh* mesh, const std::vector<Material*>& materials, const Transform& transform)
	{
		size_t transformIndex = Ms.size();
		glm::mat4 M = transform.CalcMatrix();
		Ms.push_back(M);
		MVPs.push_back(VP * M);

		for (size_t i = 0; i < mesh->primitiveGroups.size() && i < materials.size(); i++)
		{
			Material* material = materials[i];
			Shader* shader = material->shader;
			
			if (shaderGroups.count(shader) == 0)
				shaderGroups[shader] = {};

			ShaderGroup& sg = shaderGroups[shader];

			if (sg.subMeshGroups.count(mesh) == 0)
				sg.subMeshGroups[mesh] = {};

			std::vector<SubMeshGroup>& smgs = sg.subMeshGroups[mesh];
			smgs.push_back({material, transformIndex, i});
		}
	}

	void Renderer::Render()
	{
		for (auto& shaderGroup : shaderGroups)
		{
			Shader* shader = shaderGroup.first;
			shader->Use();

			for (auto& subMeshGroups : shaderGroup.second.subMeshGroups)
			{
				Mesh* mesh = subMeshGroups.first;
				mesh->Bind();

				for (auto& subMeshGroup : subMeshGroups.second)
				{
					Material* material = subMeshGroup.material;
					glm::mat4& M = Ms[subMeshGroup.transformIndex];
					glm::mat4& MVP = MVPs[subMeshGroup.transformIndex];

					material->Apply({cameraTransform, M, MVP});

					mesh->Draw(subMeshGroup.subMeshIndex);

					material->CleanUp();
				}

				mesh->Unbind();
			}

			shader->StopUsing();
		}

		shaderGroups.clear();
		Ms.clear();
		MVPs.clear();
	}
}