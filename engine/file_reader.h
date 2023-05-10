#pragma once
#include <string>
#include <vector>
#include "mesh.h"
#include "material.h"

namespace Engine
{
	struct ObjMaterialInfo
	{
		glm::vec3 color;
	};

	bool ReadTextFile(const std::string& path, std::string& text);
	bool LoadOBJFile(Mesh& outMesh, std::vector<ObjMaterialInfo>& outMatInfo, const std::string& path);
}