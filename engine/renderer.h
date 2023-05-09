#pragma once
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <unordered_map>
#include <functional>

namespace Engine
{
	struct Material
	{
		Shader* shader;
		std::function<void(Shader*)> Bind;
		std::function<void(Shader*)> Unbind;
	};

	class Renderer
	{
	private:
		std::vector<glm::mat4> transforms;

		struct RenderNode
		{
			Mesh* mesh;
			size_t primitiveGroupIndex;
			size_t transformIndex;
			std::function<void(Shader*)> Bind;
			std::function<void(Shader*)> Unbind;
		};

		std::unordered_map<Shader*, std::vector<RenderNode>> renderNodes;

	public:
		Renderer();
		~Renderer();

		void Draw(Mesh* mesh, const std::vector<Material>& materials, const Transform& transform);

		void Render(const Camera& camera, const Transform& cameraTransform);
	};
}