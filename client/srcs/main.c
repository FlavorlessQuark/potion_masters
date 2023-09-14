#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;
SDLX_RectContainer *root;

typedef void (*loop)(Context *);

loop fnloops[3] ={main_screen, board_screen, connect_screen};



void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_RenderQueueFlushAll();
	// window_events(&ctx);
	SDLX_InputUpdate();
	// SDLX_ContainerUpdate(root, NULL);
	//
	SDLX_ButtonUpdate();

	// SDLX_DisplayConfig(ctx.display->renderer, root);
	// if (ctx.connection.hasMessage == SDL_TRUE)
	// {
	// 	// SDL_Log("Received a message %s %p",  ctx.connection.message, ctx.connection.message);
	// 	parse_message(ctx.connection.message);
	// 	ctx.connection.hasMessage = SDL_FALSE;
	// }
	// fnloops[ctx.state](&ctx);
	board_screen(&ctx);
	// SDLX_RenderAll(ctx.display);
	// SDL_RenderCopy(ctx.display->renderer, ctx.switchSprite.texture, NULL, NULL);
	SDL_RenderPresent(ctx.display->renderer);
}

int main(int argc, char **argv)
{
	// printf("ARguemnts %d\n", argc);

	// for (int i = 0; i < argc; i++)
	// 	printf("Argument %d : %s\n", i, argv[i]);
	init_static(&ctx, SDL_atoi(argv[1]), SDL_atoi(argv[2]));
	root = SDLX_LoadConfig("assets/UI/recipeUI");
	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(core, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			core();
	#endif
}
