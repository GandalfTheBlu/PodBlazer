#pragma once
#include "shader.h"
#include "mesh.h"
#include "material.h"
#include "transform.h"
#include "camera.h"
#include <unordered_map>
#include <functional>

namespace Engine
{
	class Renderer
	{
	private:
		glm::mat4 cameraTransform;
		glm::mat4 VP;
		std::vector<glm::mat4> Ms;
		std::vector<glm::mat4> MVPs;

		Mesh skyboxMesh;
		Material skyboxMaterial;

		struct RenderNode
		{
			Mesh* mesh;
			size_t primitiveGroupIndex;
			size_t transformIndex;
			MaterialBindFunction Bind;
			MaterialUnbindFunction Unbind;
		};

		std::unordered_map<Shader*, std::vector<RenderNode>> renderNodes;

	public:
		Renderer();
		~Renderer();

		void Init(const Material& _skyboxMaterial);
		void Deinit();

		void SetCamera(const Camera& camera, const Transform& _cameraTransform);
		void Draw(Mesh* mesh, const std::vector<Material>& materials, const Transform& transform);
		void Render();
	};
}