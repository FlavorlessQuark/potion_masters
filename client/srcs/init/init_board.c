#include "splendor.h"

# define ROW_CARD_COUNT 4
# define MASTER_CARD_COUNT 3
# define CARDS 1

void initMasterPotions(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	for (int i = 0; i < MASTER_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		SDLX_SpriteCreate(&row->card[i].sprite, 1, ctx->assets.cardTex);

		row->cardButton[i]._boundingBox = root->elems[i].boundingBox;
		row->card[i].sprite._dst = root->elems[i].boundingBox;
		row->card[i].sprite.src = NULL;
		row->card[i].sprite.texture = NULL;
	}
}

void initRowPotions(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	// Parse texture data here
	// row->rowIcon.texture = ctx->cardTex;
	// get_img_src(&row->rowIcon._src, CARD_BACK, 0);
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		SDLX_SpriteCreate(&row->card[i].sprite, 1, ctx->assets.cardTex);

		row->cardButton[i]._boundingBox = root->elems[i].boundingBox;
		row->card[i].sprite._dst = root->elems[i].boundingBox;
		row->card[i].sprite.src = NULL;
		row->card[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, row->card[i].sprite._dst.w, row->card[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->card[i].sprite.texture , SDL_BLENDMODE_BLEND);
	}

}

void init_board_static(Context *ctx)
{
	SDL_Surface *surf;
	SDLX_RectContainer *root;

	root = SDLX_LoadConfig("assets/UI/boardUI");

	// ctx->board.bg = SDL_CreateTexture(ctx->display->renderer,
	// 	SDL_PIXELFORMAT_RGBA8888,
	// 	SDL_TEXTUREACCESS_TARGET,
	// 	ctx->display->win_w, ctx->display->win_h
	// 	);
	// SDL_SetRenderTarget(ctx->display->renderer, ctx->board.bg);

	// initTokens(ctx, &root->containers[TOKENS]);
	initMasterPotions(ctx, &ctx->board.masterPotions, &root->containers[CARDS].containers[MASTER_ROW]);
	initRowPotions(ctx, &ctx->board.rows[0], &root->containers[CARDS].containers[LVL2_ROW]);
	initRowPotions(ctx, &ctx->board.rows[1], &root->containers[CARDS].containers[LVL1_ROW]);
	initRowPotions(ctx, &ctx->board.rows[2], &root->containers[CARDS].containers[BASE_ROW]);

	// surf = IMG_Load("assets/cards.png");
	SDLX_SpriteCreate(&ctx->board.switchScreen.sprite, 1, NULL);
	SDLX_ButtonCreate(&ctx->board.switchScreen.button, &ctx->board.switchScreen.sprite._dst);

	ctx->board.switchScreen.sprite._dst = root->containers[0].elems[0].boundingBox;
	// ctx->switchSprite.texture =
	// ctx->switchSprite._dst = root->containers[SWITCH].elems[0]._boundingBox;
	// ctx->switchSprite.src = NULL;
	// SDL_SetRenderTarget(ctx->display->renderer, ctx->switchSprite.texture);
	// SDLX_RenderMessage(ctx->display, NULL, (SDL_Color){255,255,255,255}, "->");
	// SDL_FreeSurface(surf);

	SDL_SetRenderTarget(ctx->display->renderer, NULL);
}
