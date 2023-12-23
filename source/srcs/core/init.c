#include "game.h"


void init_textures(Context *ctx);

void init_game(Context *ctx, int width, int height)
{

	SDL_Surface *surf;
	SDLX_RectContainer *root;

    SDLX_InitDefault();
	// SDLX_Init("SDLX_Default", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	set_resize_context(ctx);
	ctx->display = SDLX_DisplayGet();

	init_textures(ctx);
	ctx->display->defaultFont = TTF_OpenFont("assets/wizzta.ttf", 80);
\
	init_connect_screen(ctx);
	init_main_screen(ctx);
	init_board_static(ctx);
	initConnection(ctx);

	fill_main_screen(ctx);
	fill_board(ctx);
	fill_connect_screen(ctx);
	reset_game(ctx);
}

void init_textures(Context *ctx)
{
	SDL_Surface *surf;

	// SDL_Log("Preparing textures...");

	surf = IMG_Load("assets/potions.png");
	ctx->assets.potions = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/ui_p1.png");
	ctx->assets.texUI = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/potionbg.png");
	ctx->assets.overlayBG = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/essences.png");
	ctx->assets.essences = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/main_bg.png");
	ctx->assets.main_bg = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/board_bg.png");
	ctx->assets.board_bg = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/shelves.png");
	ctx->assets.shelf = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);

	surf = IMG_Load("assets/recipe.png");
	ctx->assets.recipe = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);
}
