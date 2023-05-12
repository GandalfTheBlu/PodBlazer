#include "materials.h"
#include "../exts/glm/matrix.hpp"
#include "world_settings.h"

namespace Game
{
	SkyboxMaterial::SkyboxMaterial(){}

	void SkyboxMaterial::Apply(const Engine::MaterialInput& input)
	{
		glm::mat3 invMVP = glm::inverse(glm::mat3(input.MVP));
		shader->SetMat3("u_invMVP", &invMVP[0][0]);
		shader->SetVec3("u_lightDir", &WorldSettings::Instance().directionalLight[0]);
		glDepthMask(GL_FALSE);
	}

	void SkyboxMaterial::CleanUp()
	{
		glDepthMask(GL_TRUE);
	}

	FlatMaterial::FlatMaterial() :
		color(glm::vec3(1.f, 0.f, 1.f)),
		roughness(0.5f)
	{}

	void FlatMaterial::Apply(const Engine::MaterialInput& input)
	{
		glm::mat3 N = glm::transpose(glm::inverse(glm::mat3(input.M)));
		glm::vec3 camPos = input.camTransform[3];

		shader->SetMat3("u_N", &N[0][0]);
		shader->SetMat4("u_M", &input.M[0][0]);
		shader->SetMat4("u_MVP", &input.MVP[0][0]);
		shader->SetVec3("u_camPos", &camPos[0]);
		shader->SetVec3("u_color", &color[0]);
		shader->SetFloat("u_roughness", roughness);
		shader->SetVec3("u_lightDir", &WorldSettings::Instance().directionalLight[0]);
	}

	TextureMaterial::TextureMaterial() :
		roughness(0.5f),
		texture(nullptr)
	{}

	void TextureMaterial::Apply(const Engine::MaterialInput& input)
	{
		glm::mat3 N = glm::transpose(glm::inverse(glm::mat3(input.M)));
		glm::vec3 camPos = input.camTransform[3];

		shader->SetMat3("u_N", &N[0][0]);
		shader->SetMat4("u_M", &input.M[0][0]);
		shader->SetMat4("u_MVP", &input.MVP[0][0]);
		shader->SetVec3("u_camPos", &camPos[0]);
		shader->SetFloat("u_roughness", roughness);
		shader->SetVec3("u_lightDir", &WorldSettings::Instance().directionalLight[0]);

		texture->Bind(GL_TEXTURE0);
	}

	void TextureMaterial::CleanUp()
	{
		texture->Unbind(GL_TEXTURE0);
	}
}