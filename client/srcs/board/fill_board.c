#include "splendor.h"

# define ROW_CARD_COUNT 4
# define MASTER_CARD_COUNT 3
# define CARDS 1

static SDLX_RectContainer *_root = NULL;
static SDLX_RectContainer *_overlayRoot = NULL;

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

void fillOverlay(Context *ctx, SDLX_RectContainer *root)
{
	SDLX_RectContainer *base;

	base = &root->containers[0].containers[0];

	ctx->board.overlay.background._dst  = root->self.boundingBox;
	ctx->board.overlay.nameDst 			= base->containers[0].elems[0].boundingBox;
	ctx->board.overlay.potion._dst 		= base->containers[1].containers[0].elems[0].boundingBox;
	ctx->board.overlay.costDst 			= base->containers[1].containers[0].elems[1].boundingBox;
	ctx->board.overlay.essences[0]._dst = base->containers[1].containers[0].elems[2].boundingBox;
	ctx->board.overlay.descDst 			= base->containers[1].containers[1].elems[0].boundingBox;

	ctx->board.overlay.exit.sprite._dst.x = root->self.boundingBox.x + root->self.boundingBox.w;
	ctx->board.overlay.exit.sprite._dst.y = root->self.boundingBox.y;
	ctx->board.overlay.exit.sprite._dst.w = root->self.boundingBox.w / 5;
	ctx->board.overlay.exit.sprite._dst.h = root->self.boundingBox.w / 5;
	SDLX_SpritePrint(&ctx->board.overlay.exit.sprite);
	SDL_Log("S2) hould be %p, is %p", &ctx->board.overlay.exit.sprite._dst, ctx->board.overlay.exit.button.boundingBox);
}

void fillRowPotions(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	// Parse texture data here
	// row->rowIcon.texture = ctx->cardTex;
	// get_img_src(&row->rowIcon._src, CARD_BACK, 0);
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{

		// row->cardButton[i]._boundingBox = root->elems[i].boundingBox;
		row->card[i].sprite._dst = root->elems[i].boundingBox;
		row->card[i].sprite.src = NULL;
	}

}

void fill_board(Context *ctx)
{
	SDL_Surface *surf;

	if (!_root)
	{
		_root = SDLX_LoadConfig("assets/UI/boardUI");
		_overlayRoot = SDLX_LoadConfig("assets/UI/recipeUI");
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 3;
	}
	else
	{
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 3;
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}

	// ctx->board.bg = SDL_CreateTexture(ctx->display->renderer,
	// 	SDL_PIXELFORMAT_RGBA8888,
	// 	SDL_TEXTUREACCESS_TARGET,
	// 	ctx->display->win_w, ctx->display->win_h
	// 	);
	// SDL_SetRenderTarget(ctx->display->renderer, ctx->board.bg);

	fillMasterPotions(ctx, &ctx->board.masterPotions, &_root->containers[CARDS].containers[MASTER_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[0], &_root->containers[CARDS].containers[LVL2_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[1], &_root->containers[CARDS].containers[LVL1_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[2], &_root->containers[CARDS].containers[BASE_ROW]);

	fillOverlay(ctx, _overlayRoot);

	// surf = IMG_Load("assets/cards.png");

	ctx->board.switchScreen.sprite._dst = _root->containers[0].elems[0].boundingBox;
	SDLX_SpritePrint(&ctx->board.overlay.background);

	// SDL_SetRenderTarget(ctx->display->renderer, NULL);
}
