#pragma once
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "../exts/glm/matrix.hpp"
#include "../exts/glm/vec2.hpp"
#include <unordered_map>
#include <string>

namespace Game
{
	class TextRenderer
	{
	private:
		Engine::Shader* shader;
		Engine::Texture* fontTexture;
		Engine::Mesh* screenQuad;
		glm::vec2 fontSize;

		std::unordered_map<char, glm::vec2> charToPosition;

	public:
		TextRenderer();

		void Init(Engine::Shader* _shader, Engine::Texture* _fontTexture, Engine::Mesh* _screenQuad, const glm::vec2& _fontSize);

		void DrawText(const std::string& text, const glm::mat4& transform, const glm::vec3& color, bool checkDepth = false);
	};
}