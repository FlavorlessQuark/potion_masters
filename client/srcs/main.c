#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;

typedef void (*loop)(Context *);

loop fnloops[3] ={main_screen, board_screen, buy_screen};

void init(Context *ctx)
{
	SDLX_RectContainer *root;

	SDLX_Init("Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
	ctx->display = SDLX_DisplayGet();
	init_connect_screen(ctx);
	init_main_screen(ctx);
	init_board_screen(ctx);
	init_buy_screen(ctx);
	initConnection(ctx);
	ctx->state = 0;
}


void core(void)
{
	SDLX_RenderReset(ctx.display);
	window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ButtonUpdate();
	if (ctx.connection.hasMessage == SDL_TRUE)
	{
		SDL_Log("Received a message %s %p",  ctx.connection.message, ctx.connection.message);
		parseMsg(&ctx, ctx.connection.message);
	}
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
