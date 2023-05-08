#include "window.h"

namespace Game
{
	class App
	{
	private:
		Engine::Window window;
		bool shouldClose;

	public:
		App();
		~App();

		bool Open();
		void UpdateLoop();
		void Close();
	};
}