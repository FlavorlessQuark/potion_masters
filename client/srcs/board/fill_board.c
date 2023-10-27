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
	ctx->board.overlay.name._dst 		= base->containers[0].elems[0].boundingBox;
	ctx->board.overlay.potion._dst 		= base->containers[1].containers[0].elems[0].boundingBox;
	ctx->board.overlay.cost._dst		= base->containers[1].containers[0].elems[1].boundingBox;
	// ctx->board.overlay.essences[0]._dst = base->containers[1].containers[0].elems[2].boundingBox;
	ctx->board.overlay.desc._dst		= base->containers[1].containers[1].elems[0].boundingBox;

	// ctx->board.overlay.name.texture 	= create_target_texture(ctx->board.overlay.name._dst.w, ctx->board.overlay.name._dst.h);
	ctx->board.overlay.name.texture 	= create_target_texture(ctx->board.overlay.name._dst.w, ctx->board.overlay.name._dst.h);
	ctx->board.overlay.potion.texture 	= create_target_texture(ctx->board.overlay.potion._dst.w, ctx->board.overlay.potion._dst.h);
	ctx->board.overlay.cost.texture 	= create_target_texture(ctx->board.overlay.cost._dst.w, ctx->board.overlay.cost._dst.h);
	ctx->board.overlay.desc.texture 	= create_target_texture(ctx->board.overlay.desc._dst.w, ctx->board.overlay.desc._dst.h);


	overlay_text(ctx->board.overlay.cost.texture , NULL, NULL, WHITE, 1, "Cost : ");
	SDL_Rect bounds = base->containers[1].containers[0].elems[2].boundingBox;
	bounds.w /= ESSENCE_TYPES;
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		ctx->board.overlay.essences[i].texture = create_target_texture(bounds.w, bounds.h);
		ctx->board.overlay.essences[i]._dst = bounds;
		ctx->board.overlay.essences[i].src = NULL;
		bounds.x += bounds.w;
	}

	ctx->board.overlay.exit.sprite._dst.x = root->self.boundingBox.x + root->self.boundingBox.w + 50;
	ctx->board.overlay.exit.sprite._dst.y = root->self.boundingBox.y;
	ctx->board.overlay.exit.sprite._dst.w = root->self.boundingBox.w / 5;
	ctx->board.overlay.exit.sprite._dst.h = root->self.boundingBox.w / 5;

	ctx->board.overlay.buy.sprite._dst.x = root->self.boundingBox.x + root->self.boundingBox.w + 50;
	ctx->board.overlay.buy.sprite._dst.y = root->self.boundingBox.y + ( root->self.boundingBox.w / 2);
	ctx->board.overlay.buy.sprite._dst.w = root->self.boundingBox.w / 3;
	ctx->board.overlay.buy.sprite._dst.h = root->self.boundingBox.h / 5;
	ctx->board.overlay.buy.sprite.texture 	= create_target_texture(ctx->board.overlay.buy.sprite._dst.w, ctx->board.overlay.buy.sprite._dst.h);

	overlay_text(ctx->board.overlay.buy.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, BLACK, 0.6, "Buy");
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
		if (row->card[i].sprite.texture)
			SDL_DestroyTexture(row->card[i].sprite.texture);
		row->card[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, row->card[i].sprite._dst.w, row->card[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->card[i].sprite.texture , SDL_BLENDMODE_BLEND);
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
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}
	else
	{
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 3;
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}
	fillMasterPotions(ctx, &ctx->board.masterPotions, &_root->containers[CARDS].containers[MASTER_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[0], &_root->containers[CARDS].containers[LVL2_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[1], &_root->containers[CARDS].containers[LVL1_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[2], &_root->containers[CARDS].containers[BASE_ROW]);

	fillOverlay(ctx, _overlayRoot);
	ctx->board.switchScreen.sprite._dst = _root->containers[0].elems[0].boundingBox;
}
