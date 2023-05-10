#include "renderer.h"

namespace Engine
{
	Renderer::Renderer(){}
	Renderer::~Renderer(){}

	void Renderer::Init(const Material& _skyboxMaterial)
	{
		skyboxMesh.ScreenQuad();
		skyboxMaterial = _skyboxMaterial;
	}

	void Renderer::Deinit()
	{
		skyboxMesh.Deinit();
	}

	void Renderer::SetCamera(const Camera& camera, const Transform& _cameraTransform)
	{
		cameraTransform = _cameraTransform.CalcMatrix();
		VP = camera.CalcP() * camera.CalcV(_cameraTransform);
	}

	void Renderer::Draw(Mesh* mesh, const std::vector<Material>& materials, const Transform& transform)
	{
		size_t transformIndex = Ms.size();
		glm::mat4 M = transform.CalcMatrix();
		Ms.push_back(M);
		MVPs.push_back(VP * M);

		for (size_t i = 0; i < mesh->primitiveGroups.size() && i < materials.size(); i++)
		{
			Shader* shader = materials[i].shader;
			
			if (renderNodes.count(shader) == 0)
				renderNodes[shader] = {};

			renderNodes[shader].push_back({mesh, i, transformIndex, materials[i].Bind, materials[i].Unbind});
		}
	}

	void Renderer::Render()
	{
		// draw skybox
		glDepthMask(GL_FALSE);
		skyboxMaterial.shader->Use();
		skyboxMaterial.Bind({skyboxMaterial.shader, cameraTransform, glm::mat4(), VP});

		skyboxMesh.Bind();
		skyboxMesh.Draw(0);
		skyboxMesh.Unbind();

		if(skyboxMaterial.Unbind != nullptr)
			skyboxMaterial.Unbind();

		skyboxMaterial.shader->StopUsing();
		glDepthMask(GL_TRUE);


		// draw geometry
		for (auto& elem : renderNodes)
		{
			Shader* shader = elem.first;
			shader->Use();

			for (auto& renderNode : elem.second)
			{
				glm::mat4& M = Ms[renderNode.transformIndex];
				glm::mat4& MVP = MVPs[renderNode.transformIndex];

				renderNode.mesh->Bind();
				renderNode.Bind({ shader, cameraTransform, M, MVP});

				renderNode.mesh->Draw(renderNode.primitiveGroupIndex);

				if(renderNode.Unbind != nullptr)
					renderNode.Unbind();

				renderNode.mesh->Unbind();
			}

			shader->StopUsing();
		}

		renderNodes.clear();
		Ms.clear();
		MVPs.clear();
	}
}