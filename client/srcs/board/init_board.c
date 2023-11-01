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
	SDLX_SpriteCreate(&ctx->board.overlay.background, 1, ctx->assets.overlayBG);
	SDLX_SpriteCreate(&ctx->board.overlay.potion, 1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.name, 1, NULL);
	// SDLX_SpriteCreate(&ctx->board.overlay.fill, 1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.desc, 1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.cost, 1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.buy.sprite,1, NULL);
	SDLX_SpriteCreate(&ctx->board.overlay.exit.sprite, 1, ctx->assets.texUI);

	ctx->board.overlay.background.src = NULL;
	ctx->board.overlay.background.dst = NULL;
	ctx->board.overlay.desc.src = NULL;
	ctx->board.overlay.name.src = NULL;
	ctx->board.overlay.cost.src = NULL;
	ctx->board.overlay.buy.sprite.src = NULL;
	// overlay_text(ctx->board.overlay.buy.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, WHITE, 0.6, "Use");
	ctx->board.overlay.exit.sprite._src = (SDL_Rect){.x = 485, .y = 0, .w = 140, .h = 155};


	SDLX_ButtonCreate(&ctx->board.overlay.buy.button, &ctx->board.overlay.buy.sprite._dst);
	SDLX_ButtonCreate(&ctx->board.overlay.exit.button, &ctx->board.overlay.exit.sprite._dst);

	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->board.overlay.essences[i], 1, NULL);
		ctx->board.overlay.essences[i].src = NULL;
	}

}

void initRowPotions(Context *ctx, Row *row)
{
	// Parse texture data here
	// row->rowIcon.texture = ctx->cardTex;
	// get_img_src(&row->rowIcon._src, CARD_BACK, 0);
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_SpriteCreate(&row->card[i].sprite, 1, NULL);
		SDLX_ButtonCreate(&row->cardButton[i], &row->card[i].sprite._dst);
		row->card[i].sprite.src = NULL;
	}

}

void init_board_static(Context *ctx)
{
	initMasterPotions(ctx, &ctx->board.masterPotions);
	initRowPotions(ctx, &ctx->board.rows[0]);
	initRowPotions(ctx, &ctx->board.rows[1]);
	initRowPotions(ctx, &ctx->board.rows[2]);


	initRecipeOverlay(ctx);

	SDLX_SpriteCreate(&ctx->board.switchScreen.sprite, 1,  ctx->assets.texUI);
	SDLX_ButtonCreate(&ctx->board.switchScreen.button, &ctx->board.switchScreen.sprite._dst);
	ctx->board.switchScreen.sprite._src = (SDL_Rect){.x = 640, .y = 0, .w = 140, .h = 155};
	ctx->board.switchScreen.sprite.flip = SDL_FLIP_HORIZONTAL;
}
