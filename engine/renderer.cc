#include "renderer.h"

namespace Engine
{
	Renderer::Renderer(){}
	Renderer::~Renderer(){}

	void Renderer::Draw(Mesh* mesh, const std::vector<Material>& materials, const Transform& transform)
	{
		size_t transformIndex = transforms.size();
		transforms.push_back(transform.CalcMatrix());

		for (size_t i = 0; i < mesh->primitiveGroups.size() && i < materials.size(); i++)
		{
			Shader* shader = materials[i].shader;
			
			if (renderNodes.count(shader) == 0)
				renderNodes[shader] = {};

			renderNodes[shader].push_back({mesh, i, transformIndex, materials[i].Bind, materials[i].Unbind});
		}
	}

	void Renderer::Render(const Camera& camera, const Transform& cameraTransform)
	{
		glm::mat4 VP = camera.CalcVP(cameraTransform);

		for (auto& elem : renderNodes)
		{
			Shader* shader = elem.first;
			shader->Use();

			for (auto& renderNode : elem.second)
			{
				renderNode.mesh->Bind();
				renderNode.Bind(shader);

				glm::mat4 MVP = VP * transforms[renderNode.transformIndex];
				shader->SetMat4("u_MVP", &MVP[0][0]);

				renderNode.mesh->Draw(renderNode.primitiveGroupIndex);

				renderNode.Unbind(shader);
				renderNode.mesh->Unbind();
			}

			shader->StopUsing();
		}

		renderNodes.clear();
	}
}