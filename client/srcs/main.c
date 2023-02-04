#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;

typedef void (*loop)(Context *);

loop fnloops[3] ={main_screen, board_screen, buy_screen};

void core(void)
{
	SDLX_RenderReset(ctx.display);
	window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ButtonUpdate();
	fnloops[ctx.state](&ctx);
	SDL_RenderPresent(ctx.display->renderer);
}

int main()
{
	init(&ctx);
	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(core, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			core();
	#endif
}
