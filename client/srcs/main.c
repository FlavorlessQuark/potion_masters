#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;

void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_InputLoop();
	SDLX_InputUpdate();
	SDLX_ButtonUpdate();
	if (ctx.state == CONNECT)
		ConnectScreen(&ctx);
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
