#include "text_renderer.h"

namespace Game
{
	TextRenderer::TextRenderer() :
		shader(nullptr),
		fontTexture(nullptr),
		screenQuad(nullptr),
		fontSize(glm::vec2(0.f))
	{}

	void TextRenderer::Init(Engine::Shader* _shader, Engine::Texture* _fontTexture, Engine::Mesh* _screenQuad, const glm::vec2& _fontSize)
	{
		shader = _shader;
		fontTexture = _fontTexture;
		screenQuad = _screenQuad;
		fontSize = _fontSize;

		const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,:!?";
		int width = fontTexture->Width();
		int height = fontTexture->Height();

		int countX = (float)width / fontSize.x;
		int countY = (float)height / fontSize.y;

		for (int j = 0; j < countY; j++)
		{
			for (int i = 0; i < countX; i++)
			{
				int index = j * countX + i;
				glm::vec2 pos = fontSize * glm::vec2(i, countY - j - 1);

				charToPosition[chars[index]] = pos;
			}
		}
	}

	void TextRenderer::DrawText(const std::string& text, const Engine::Transform& transform, const glm::vec3& color)
	{
		Engine::Transform charTransform = transform;

		shader->Use();
		fontTexture->Bind(GL_TEXTURE0);
		glDepthMask(GL_FALSE);
		screenQuad->Bind();

		glm::vec2 texSize = glm::vec2(fontTexture->Width(), fontTexture->Height());
		glm::mat4 M = charTransform.CalcMatrix();

		int line = 0;
		int col = 0;
		
		for (size_t i = 0; i < text.size(); i++)
		{
			char c = text[i];

			if (c == '\n')
			{
				line++;
				col = 0;
			}

			if (charToPosition.count(c) == 0)
				continue;
			
			glm::vec2 pos = charToPosition[c] / texSize;
			glm::vec2 size = fontSize / texSize;
			// move one screen width to the right for each character
			// and let the scale handle the rest
			glm::vec2 localOffset = glm::vec2(col * 2.f, -line * 2.f);

			shader->SetMat4("u_M", &M[0][0]);
			shader->SetVec2("u_localOffset", &localOffset[0]);
			shader->SetVec2("u_charPosition", &pos[0]);
			shader->SetVec2("u_charSize", &size[0]);
			shader->SetVec3("u_color", &color[0]);

			screenQuad->Draw(0);

			col++;
		}

		screenQuad->Unbind();
		fontTexture->Unbind(GL_TEXTURE0);
		glDepthMask(GL_TRUE);
		shader->StopUsing();
	}
}