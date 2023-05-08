#include "game_app.h"

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

		std::shared_ptr<Engine::GraphicsObject> quad = std::make_shared<Engine::GraphicsObject>();
		quad->mesh = renderer.GetMesh("default::screen_quad");
		quad->materials.push_back(
		{
			{1.f, 1.f, 1.f},
			0.f,
			renderer.GetTexture("assets\\textures\\test.png"),
			renderer.GetShader("assets\\shaders\\full_screen")
		});
		renderer.AddObject(quad);

		return true;
	}

	void App::UpdateLoop()
	{
		while (!shouldClose)
		{
			window.BeginUpdate();
			if (window.ShouldClose())
			{
				shouldClose = true;
				break;
			}
			
			renderer.Render();

			window.EndUpdate();
		}
	}

	void App::Close()
	{
		window.Deinit();
	}
}