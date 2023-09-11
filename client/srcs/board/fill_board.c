#include "splendor.h"

# define ROW_CARD_COUNT 4
# define MASTER_CARD_COUNT 3
# define CARDS 1

static SDLX_RectContainer *root = NULL;

void fillMasterPotions(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	for (int i = 0; i < MASTER_CARD_COUNT; i++)
	{
		row->cardButton[i]._boundingBox = root->elems[i].boundingBox;
		row->card[i].sprite._dst = root->elems[i].boundingBox;
		row->card[i].sprite.src = NULL;
		row->card[i].sprite.texture = NULL;
	}
}

void fillRowPotions(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	// Parse texture data here
	// row->rowIcon.texture = ctx->cardTex;
	// get_img_src(&row->rowIcon._src, CARD_BACK, 0);
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{

		row->cardButton[i]._boundingBox = root->elems[i].boundingBox;
		row->card[i].sprite._dst = root->elems[i].boundingBox;
		row->card[i].sprite.src = NULL;
	}

}

void fill_board(Context *ctx)
{
	SDL_Surface *surf;

	if (!root)
		root = SDLX_LoadConfig("assets/UI/boardUI");
	else
		SDLX_ContainerUpdate(root, NULL);

	// ctx->board.bg = SDL_CreateTexture(ctx->display->renderer,
	// 	SDL_PIXELFORMAT_RGBA8888,
	// 	SDL_TEXTUREACCESS_TARGET,
	// 	ctx->display->win_w, ctx->display->win_h
	// 	);
	// SDL_SetRenderTarget(ctx->display->renderer, ctx->board.bg);

	// fillTokens(ctx, &root->containers[TOKENS]);
	fillMasterPotions(ctx, &ctx->board.masterPotions, &root->containers[CARDS].containers[MASTER_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[0], &root->containers[CARDS].containers[LVL2_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[1], &root->containers[CARDS].containers[LVL1_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[2], &root->containers[CARDS].containers[BASE_ROW]);

	// surf = IMG_Load("assets/cards.png");

	ctx->board.switchScreen.sprite._dst = root->containers[0].elems[0].boundingBox;
	// ctx->switchSprite.texture =
	// ctx->switchSprite._dst = root->containers[SWITCH].elems[0]._boundingBox;
	// ctx->switchSprite.src = NULL;
	// SDL_SetRenderTarget(ctx->display->renderer, ctx->switchSprite.texture);
	// SDLX_RenderMessage(ctx->display, NULL, (SDL_Color){255,255,255,255}, "->");
	// SDL_FreeSurface(surf);

	// SDL_SetRenderTarget(ctx->display->renderer, NULL);
}
