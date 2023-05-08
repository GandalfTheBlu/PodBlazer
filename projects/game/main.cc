#include "game_app.h"

int main()
{
	Game::App app;
	if (!app.Open())
		return 1;
	
	app.UpdateLoop();
	app.Close();

	return 0;
}