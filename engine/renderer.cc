#include "renderer.h"

namespace Engine
{
	Renderer::Renderer() : 
		resolutionX(0),
		resolutionY(0),
		framebuffer(0),
		colorBuffer(0),
		depthBuffer(0),
		cameraTransform(glm::mat4()),
		VP(glm::mat4())
	{}

	Renderer::~Renderer(){}

	bool Renderer::Init(GLsizei _resolutionX, GLsizei _resolutionY)
	{
		resolutionX = _resolutionX;
		resolutionY = _resolutionY;

		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		glViewport(0, 0, resolutionX, resolutionY);

		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, resolutionX, resolutionY, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

		GLuint attachment = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &attachment);

		// create and add a depth buffer
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, resolutionX, resolutionY);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("[ERROR] framebuffer not completed\n");
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	void Renderer::Deinit()
	{
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteTextures(1, &colorBuffer);
		glDeleteTextures(1, &depthBuffer);
	}

	void Renderer::SetCamera(const Camera& camera, const Transform& _cameraTransform)
	{
		cameraTransform = _cameraTransform.CalcMatrix();
		VP = camera.CalcP() * camera.CalcV(_cameraTransform);
	}

	void Renderer::PushDrawCall(Mesh* mesh, const std::vector<Material*>& materials, const Transform& transform)
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

	void Renderer::ExecuteDrawCalls()
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

	void Renderer::RenderToScreen(GLsizei windowWidth, GLsizei windowHeight)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, resolutionX, resolutionY, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}
}