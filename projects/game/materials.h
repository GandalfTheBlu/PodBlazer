#pragma once
#include "material.h"
#include "../exts/glm/vec2.hpp"
#include "../exts/glm/vec3.hpp"
#include "texture.h"

namespace Game
{
	class SkyboxMaterial : public Engine::Material
	{
	public:
		SkyboxMaterial();

		virtual void Apply(const Engine::MaterialInput& input) override;
		virtual void CleanUp() override;
	};

	class FlatMaterial : public Engine::Material
	{
	public:
		glm::vec3 color;
		float roughness;

		FlatMaterial();

		virtual void Apply(const Engine::MaterialInput& input) override;
	};

	class TextureMaterial : public Engine::Material
	{
	public:
		float roughness;
		Engine::Texture* texture;

		TextureMaterial();

		virtual void Apply(const Engine::MaterialInput& input) override;
		virtual void CleanUp() override;
	};
}