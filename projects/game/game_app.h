#pragma once
#include "window.h"
#include "renderer.h"

namespace Game
{
	class App
	{
	private:
		Engine::Window window;
		bool shouldClose;

		Engine::Renderer renderer;

	public:
		App();
		~App();

		bool Open();
		void UpdateLoop();
		void Close();
	};
}