#pragma once
#include "texture.h"
#include "shader.h"

class PointSystem {

private:
	int currentPoints = 0;
	//std::vector<Engine::Texture> UITextures;
	Engine::Texture UITextures[10];

	Engine::Shader UIShader;

	PointSystem();

public:

	void AddPoint();
	void GetPoints();
	void DisplayPoints();
	void SavePoints();
	void DisplayGameOver();
	void RestartGame();
	void LoadUITexture();
	void RenderUI();

};