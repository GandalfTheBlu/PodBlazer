#include "game_app.h"
#include "shader.h"
#include "mesh.h"

namespace Game
{
	App::App() :
		shouldClose(false)
	{}

	App::~App(){}

	bool App::Open()
	{
		if (!window.Init(800, 600, "Pod Blazer"))
			return false;

		return true;
	}

	void App::UpdateLoop()
	{
		Engine::Shader shader;
		if (!shader.Init("assets\\shaders\\full_screen"))
			return;

		Engine::Mesh mesh;
		Engine::ScreenQuad(mesh);

		while (!shouldClose)
		{
			window.BeginUpdate();
			if (window.ShouldClose())
			{
				shouldClose = true;
				break;
			}
			
			shader.Use();
			mesh.Bind();
			mesh.Draw(0);
			mesh.Unbind();
			shader.StopUsing();

			window.EndUpdate();
		}
	}

	void App::Close()
	{
		window.Deinit();
	}
}