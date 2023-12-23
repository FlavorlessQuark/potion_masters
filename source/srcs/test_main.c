#include "../includes/game.h"

int main()
{
	SDLX_RectContainer *root;
	SDLX_Display *display;

	SDLX_InitDefault();
	root = SDLX_LoadConfig("assets/UI/menuUI");

	// cleanupUIConfig(root);
	display = SDLX_DisplayGet();

	while (1)
	{
		SDLX_RenderReset(SDLX_DisplayGet());
		SDLX_InputLoop();
		SDLX_DisplayConfig(SDLX_DisplayGet()->renderer ,root);
		SDL_RenderPresent(SDLX_DisplayGet()->renderer);
	}
	SDLX_RenderQueuesCleanup();
}
