#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;

typedef void (*loop)(Context *);

loop fnloops[4] ={connect_screen, main_screen, board_screen, buy_screen};



void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_RenderQueueFlushAll();
	window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ButtonUpdate();
	fnloops[ctx.state](&ctx);
	SDLX_RenderAll(ctx.display);
	SDL_RenderPresent(ctx.display->renderer);
}

int main(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
		printf("Argument %d : %s\n", i, argv[i]);
	init(&ctx, SDL_atoi(argv[1]), SDL_atoi(argv[2]));
	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(core, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			core();
	#endif
}
