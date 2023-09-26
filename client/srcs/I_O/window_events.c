#include "../includes/splendor.h"

static Context *ctx;

void set_resize_context(Context *context) {ctx = context;}

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

void resizeWindow(int width, int height)
{
	SDLX_Display *display;

	SDL_Log("RESIZE EVENT %d %d -> %d %d", display->win_w, display->win_h,  width, height);

	display = SDLX_DisplayGet();
	display->win_w = width;
	display->win_h = height;
	SDL_SetWindowSize(display->window, width, height);

	fill_board(ctx);
	fill_main_screen(ctx);
	// initPositions();
}
