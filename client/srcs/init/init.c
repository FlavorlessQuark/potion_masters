#include "../includes/splendor.h"

void load_files(Context *ctx)
{
	SDL_Surface *surf;

// Load font
	ctx->display->defaultFont = TTF_OpenFont("assets/default.ttf", 100);
	SDLX_TextSheet_Create(&ctx->textSheet, ctx->display->win_w, ctx->display->win_h);
	ctx->nums = SDLX_TextSheet_Add(&ctx->textSheet ,"0123456789", ctx->display->defaultFont, (SDL_Color){255,255,255,255});
	TTF_SizeText(ctx->display->defaultFont, "0", &ctx->nums.w, &ctx->nums.h);

// Load textures
	surf = IMG_Load("assets/cards.png");
	ctx->cardTex  = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_FreeSurface(surf);
	surf = IMG_Load("assets/buttons.png");
	ctx->buttonTex  = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_Log("IMAGE SOZE IS % d% d", surf->w, surf->h);
	SDL_FreeSurface(surf);
}

void init(Context *ctx, int width, int height)
{

	SDLX_Init("Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	ctx->display = SDLX_DisplayGet();

	load_files(ctx);
	init_connect_screen(ctx);
	init_main_screen(ctx);
	init_board_screen(ctx);
	init_buy_screen(ctx);
	initConnection(ctx);
	endTurn(ctx);
	ctx->state = CONNECT;

	// sendMessage("Test");
}
