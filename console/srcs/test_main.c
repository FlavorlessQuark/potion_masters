#include "../includes/splendor.h"

int main()
{
	SDLX_RectContainer *root;
	SDLX_Display *display;

	SDLX_InitDefault();
	root = loadConfig("assets/UI/playerUI");

	// cleanupUIConfig(root);
	display = SDLX_DisplayGet();

	while (1)
	{
		SDLX_RenderReset(SDLX_DisplayGet());
		SDLX_InputLoop();
		print_config(SDLX_DisplayGet()->renderer ,root);
		SDL_RenderPresent(SDLX_DisplayGet()->renderer);
	}
	SDLX_RenderQueuesCleanup();
}
