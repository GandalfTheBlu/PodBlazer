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
			

			window.EndUpdate();
		}
	}

	void App::Close()
	{
		window.Deinit();
	}
}