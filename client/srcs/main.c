#include "../includes/splendor.h"
#include "../includes/SDLX/SDLX.h"
#include <emscripten.h>

Context ctx;
SDLX_RectContainer *root;

typedef void (*loop)(Context *);

loop fnloops[4] ={main_screen, board_screen, buy_screen, connect_screen};

void init(Context *ctx, int width, int height)
{

	SDL_Surface *surf;

	SDLX_Init("Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	ctx->display = SDLX_DisplayGet();

	// ctx->display->defaultFont = TTF_OpenFont("assets/default.ttf", 80);
	// SDLX_TextSheet_Create(&ctx->textSheet, ctx->display->win_w, ctx->display->win_h);
	// ctx->nums = SDLX_TextSheet_Add(&ctx->textSheet ,"0123456789", ctx->display->defaultFont, (SDL_Color){255,255,255,255});
	// TTF_SizeText(ctx->display->defaultFont, "0", &ctx->nums.w, &ctx->nums.h);
	// // TTF_CloseFont(ctx->display->defaultFont);
	// // ctx->display->defaultFont = TTF_OpenFont("assets/wizzta.ttf", 40);

	// surf = IMG_Load("assets/cards.png");
	// ctx->cardTex  = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	// SDL_FreeSurface(surf);
	root = SDLX_ParseConfig("assets/UI/testUI");
	// SDLX_Sprite costSprite[POTION_TYPES];
	// init_connect_screen(ctx);
	// init_main_screen(ctx);
	// init_board_screen(ctx);
	// init_buy_screen(ctx);
	initConnection(ctx);
	// endTurn(ctx);
	// ctx->state = CONNECT;

	// sendMessage("Test");
}

void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_RenderQueueFlushAll();
	window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ContainerUpdate(root, NULL);
	// SDLX_ButtonUpdate();

	SDLX_DisplayConfig(ctx.display->renderer, root);
	// if (ctx.connection.hasMessage == SDL_TRUE)
	// {
	// 	// SDL_Log("Received a message %s %p",  ctx.connection.message, ctx.connection.message);
	// 	parse_message(ctx.connection.message);
	// 	ctx.connection.hasMessage = SDL_FALSE;
	// }
	// fnloops[ctx.state](&ctx);
	SDLX_RenderAll(ctx.display);
	// SDL_RenderCopy(ctx.display->renderer, ctx.switchSprite.texture, NULL, NULL);
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
