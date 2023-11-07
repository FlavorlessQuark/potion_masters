#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;
SDLX_RectContainer *root;

typedef void (*loop)(Context *);

loop fnloops[3] ={main_screen, board_screen, connect_screen};

void end()
{
	SDL_Rect dst;
	SDL_Rect sdst;
	int color = 0xFFFFFFFF;
	char text[21];

	dst.x = 0;
	dst.y = 0;
	dst.w = ctx->display->win_w;
	dst.h = ctx->display->win_h;
	sdst = dst;
	if (ctx->winner == ctx.player.id)
		sprintf(text, "YOU WON");
	else
		sprintf(text, "PLAYER %d WON!", ctx->winner);
	TTF_SizeText(ctx->display->defaultFont, text, &sdst.w, &sdst.h);
	sdst = scale_and_center(0.5, dst, sdst);
	SDLX_RenderMessage(ctx->display, &sdst,(SDL_Color){.r = (color & ((uint32_t)(0xFF << 24))) >> 24, .g = (color & ((uint32_t)(0xFF << 16))) >> 16, .b = (color & ((uint32_t)(0xFF << 8))) >> 8, .a = (color & ((uint32_t)(0xFF << 0))) >> 0} , text);
}

void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_RenderQueueFlushAll();
	// window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ContainerUpdate	(root, NULL);
	//
	SDLX_ButtonUpdate();

	if (ctx.connection.hasMessage == SDL_TRUE)
	{
		// SDL_Log("Received a message %s",  ctx.connection.message);
		parse_message(&ctx, ctx.connection.message);
		ctx.connection.hasMessage = SDL_FALSE;
	}
	// fnloops[ctx.state](&ctx);
	if (ctx.state == PLAYERSTATUS)
		main_screen(&ctx);
	else if (ctx.state == BOARD)
		board_screen(&ctx);
	else if (ctx.state == CONNECT)
		connect_screen(&ctx);
	else if (ctx.state == END)
		connect_screen(&ctx);
	SDLX_RenderAll(ctx.display);
	// SDLX_DisplayConfig(ctx.display->renderer, root);
	// SDL_RenderCopy(ctx.display->renderer, ctx.switchSprite.texture, NULL, NULL);
	SDL_RenderPresent(ctx.display->renderer);
}

int main(int argc, char **argv)
{
	// printf("ARguemnts %d\n", argc);

	// for (int i = 0; i < argc; i++)
	// 	printf("Argument %d : %s\n", i, argv[i]);
	init_static(&ctx, SDL_atoi(argv[1]), SDL_atoi(argv[2]));
	root = SDLX_LoadConfig("assets/UI/playerUI");



	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(core, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			core();
	#endif
}
