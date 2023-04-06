#include "../includes/splendor.h"

void input_loop(Context *ctx)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			ctx->state = EXIT_GAME;
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			ctx->state = EXIT_GAME;
	}
}
