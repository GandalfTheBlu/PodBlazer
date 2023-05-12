#include "material.h"

namespace Engine
{
	Material::Material() :
		shader(nullptr)
	{}

	Material::~Material(){}

	void Material::CleanUp(){}
}