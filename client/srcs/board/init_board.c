#include "splendor.h"

# define ROW_CARD_COUNT 4
# define MASTER_CARD_COUNT 3
# define CARDS 1

void initMasterPotions(Context *ctx, Row *row)
{
	for (int i = 0; i < MASTER_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		SDLX_SpriteCreate(&row->card[i].sprite, 1, ctx->assets.cardTex);
	}
}

void initRecipeOverlay( Context *ctx)
{
	SDLX_SpriteCreate(&ctx->board.overlay.background, 1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.potion, 1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.buy.sprite,1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.exit.sprite, 1, NULL);

	SDLX_ButtonCreate(&ctx->board.overlay.buy.button, &ctx->board.overlay.buy.sprite._dst);
	SDLX_ButtonCreate(&ctx->board.overlay.exit.button, &ctx->board.overlay.exit.sprite._dst);


	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->board.overlay.essences[i], i, NULL);
	}

}

void initRowPotions(Context *ctx, Row *row)
{
	// Parse texture data here
	// row->rowIcon.texture = ctx->cardTex;
	// get_img_src(&row->rowIcon._src, CARD_BACK, 0);
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_SpriteCreate(&row->card[i].sprite, 1, ctx->assets.cardTex);
		SDLX_ButtonCreate(&row->cardButton[i], &row->card[i].sprite._dst);
		row->card[i].sprite.src = NULL;
		row->card[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, row->card[i].sprite._dst.w, row->card[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->card[i].sprite.texture , SDL_BLENDMODE_BLEND);
	}

}

void init_board_static(Context *ctx)
{
	initMasterPotions(ctx, &ctx->board.masterPotions);
	initRowPotions(ctx, &ctx->board.rows[0]);
	initRowPotions(ctx, &ctx->board.rows[1]);
	initRowPotions(ctx, &ctx->board.rows[2]);


	initRecipeOverlay(ctx);

	SDLX_SpriteCreate(&ctx->board.switchScreen.sprite, 1, NULL);
	SDLX_ButtonCreate(&ctx->board.switchScreen.button, &ctx->board.switchScreen.sprite._dst);
}
