#include "../includes/splendor.h"

void window_events(Context *ctx)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if  (event.type == SDL_WINDOWEVENT)
		{
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ctx->display->win_w = event.window.data1;
				ctx->display->win_h = event.window.data2;
				SDL_SetWindowSize(ctx->display->window, event.window.data1, event.window.data2);
			}
		}

	}
}
