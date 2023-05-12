#pragma once
#include "mesh.h"
#include "material.h"
#include "transform.h"
#include "camera.h"
#include <unordered_map>

namespace Engine
{
	class Renderer
	{
	private:
		glm::mat4 cameraTransform;
		glm::mat4 VP;
		std::vector<glm::mat4> Ms;
		std::vector<glm::mat4> MVPs;
		
		// rendering hierarchy:
		// for each shader in shaders
		//		for each mesh that uses shader
		//			for each primitive group in mesh that uses shader
		//				apply material
		//				draw primitive group

		struct SubMeshGroup
		{
			Material* material;
			size_t transformIndex;
			size_t subMeshIndex;
		};

		struct ShaderGroup
		{
			std::unordered_map<Mesh*, std::vector<SubMeshGroup>> subMeshGroups;
		};

		std::unordered_map<Shader*, ShaderGroup> shaderGroups;

	public:
		Renderer();
		~Renderer();

		void SetCamera(const Camera& camera, const Transform& _cameraTransform);
		void Draw(Mesh* mesh, const std::vector<Material*>& materials, const Transform& transform);
		void Render();
	};
}