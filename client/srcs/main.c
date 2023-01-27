#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;

typedef void (*loop)(Context *);

loop fnloops[2] ={board_screen, buy_screen};

void core(void)
{
	SDLX_RenderReset(ctx.display);
	window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ButtonUpdate();
	// if (ctx.state == CONNECT)
	// 	ConnectScreen(&ctx);
	// main_screen(&ctx);
	board_screen(&ctx);
	// buy_screen(&ctx);
	// fnloops[ctx.state](&ctx);
	SDL_RenderPresent(ctx.display->renderer);
}

int main()
{
	init(&ctx);
	// container = parse_UIConfig("assets/UIconfig");
	// SDLX_ContainerUpdate(container, NULL);
	// SDL_Log("------------------------");
	// SDLX_ContainerPrint(container, NULL);
	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(core, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			core();
	#endif
}
