#include "point_system.h"

PointSystem::PointSystem() {
    UIShader.Init("assets/shaders/ui");
}
void PointSystem::AddPoint()
{
}
void PointSystem::GetPoints()
{
}
void PointSystem::DisplayPoints()
{
}
void PointSystem::SavePoints()
{
}
void PointSystem::DisplayGameOver()
{
}
void PointSystem::RestartGame()
{
}
void PointSystem::LoadUITexture()
{
	for (int i = 0; i < 10; i++)
	{
		Engine::Texture texture;
		UITextures[i] = texture;
	}
	UITextures[0].Init("assets/textures/test.png");
	UITextures[1].Init("assets/textures/test.png");
	UITextures[2].Init("assets/textures/test.png");
	UITextures[3].Init("assets/textures/test.png");
	UITextures[4].Init("assets/textures/test.png");
	UITextures[5].Init("assets/textures/test.png");
	UITextures[6].Init("assets/textures/test.png");
	UITextures[7].Init("assets/textures/test.png");
	UITextures[8].Init("assets/textures/test.png");
	UITextures[9].Init("assets/textures/test.png");
}
void PointSystem::RenderUI()
{
    for (int i = 0; i < 10; i++) {
        /*
        // Bind the current UI texture to the shader
        GLint uniformLocation = glGetUniformLocation(shaderProgram, "uiTexture");
        glUniform1i(uniformLocation, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, UITextures[i].GetID());

        // Render a quad with the current UI texture
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-1.0f, 1.0f);
        glEnd();
        */
    }

}
;

