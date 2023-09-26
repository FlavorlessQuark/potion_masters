#include "splendor.h"

void init_static(Context *ctx, int width, int height)
{

	SDL_Surface *surf;
	SDLX_RectContainer *root;

	SDLX_Init("Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	set_resize_context(ctx);
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
	// root = SDLX_ParseConfig("assets/UI/boardUI");
	// SDLX_Sprite costSprite[POTION_TYPES];
	// init_connect_screen(ctx);
	// init_main_screen(ctx);
	init_board_static(ctx);
	init_main_screen(ctx);
	// init_buy_screen(ctx);
	initConnection(ctx);

	fill_board(ctx);
	fill_main_screen(ctx);
	// endTurn(ctx);
	ctx->state = PLAYERSTATUS;

	// sendMessage("Test");
}

void init_texture(Context *ctx)
{

}
