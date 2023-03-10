#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;

typedef void (*loop)(Context *);

loop fnloops[4] ={main_screen, board_screen, buy_screen, connect_screen};

void init(Context *ctx, int width, int height)
{
	SDLX_RectContainer *root;
	SDL_Surface *surf;
	SDL_Texture *tex;


	SDLX_Init("Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	ctx->display = SDLX_DisplayGet();

	ctx->display->defaultFont = TTF_OpenFont("assets/underwood.ttf", 40);
	SDLX_TextSheet_Create(&ctx->textSheet, ctx->display->win_w, ctx->display->win_h);
	ctx->nums = SDLX_TextSheet_Add(&ctx->textSheet ,"0123456789", ctx->display->defaultFont, (SDL_Color){255,255,255,255});
	TTF_SizeText(ctx->display->defaultFont, "0", &ctx->nums.w, &ctx->nums.h);

	surf = IMG_Load("assets/Cards.png");
	tex = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);
	ctx->cardTex = tex;

	init_connect_screen(ctx);
	init_main_screen(ctx);
	init_board_screen(ctx);
	init_buy_screen(ctx);
	initConnection(ctx);
	endTurn(ctx);
	ctx->state = CONNECT;

	// sendMessage("Test");
}

void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_RenderQueueFlushAll();
	window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ButtonUpdate();
	if (ctx.connection.hasMessage == SDL_TRUE)
	{
		SDL_Log("Received a message %s %p",  ctx.connection.message, ctx.connection.message);
		parseMsg(&ctx, ctx.connection.message);
	}
	fnloops[ctx.state](&ctx);
	SDLX_RenderAll(ctx.display);
	SDL_RenderPresent(ctx.display->renderer);
}

int main(int argc, char **argv)
{
	// printf("ARguemnts %d\n", argc);

	// for (int i = 0; i < argc; i++)
	// 	printf("Argument %d : %s\n", i, argv[i]);
	init(&ctx, SDL_atoi(argv[1]), SDL_atoi(argv[2]));
	#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(core, 0, 1);
	#endif
	#ifndef __EMSCRIPTEN__
		while(1)
			core();
	#endif
}
