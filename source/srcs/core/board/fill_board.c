#include "game.h"

# define ROW_CARD_COUNT 4
# define MASTER_CARD_COUNT 3
# define CARDS 1

static SDLX_RectContainer *_root = NULL;
static SDLX_RectContainer *_overlayRoot = NULL;
static int ok = 0;

void fillMasterPotions(Context *ctx, Row *row, SDLX_RectContainer *root)
{
	for (int i = 0; i < MASTER_CARD_COUNT; i++)
	{
		row->cardButton[i]._boundingBox = root->elems[i].boundingBox;
		row->card[i].sprite._dst = root->elems[i].boundingBox;
		row->card[i].sprite.src = NULL;
		if (!ok)
			row->card[i].sprite.texture = create_target_texture(row->card[i].sprite._dst.w, row->card[i].sprite._dst.h);
	}
}


//TODO FREE TEXTURES BEFORE CREATING AGAIN
void fillOverlay(Context *ctx, SDLX_RectContainer *root)
{
	SDLX_RectContainer *base;

	base = &root->containers[0].containers[1].containers[0].containers[0];

	// ctx->board.overlay.bg._dst 		= base->containers[0].containers[1].self.boundingBox;
	ctx->board.overlay.bg.dst 		= NULL;
	ctx->board.overlay.name._dst 	= base->containers[0].elems[0].boundingBox;
	ctx->board.overlay.potion._dst 	= base->containers[1].containers[0].self.boundingBox;
	ctx->board.overlay.cost._dst	= base->containers[1].containers[1].containers[0].elems[0].boundingBox;
	ctx->board.overlay.recycle._dst	= base->containers[1].containers[1].containers[1].elems[0].boundingBox;
	ctx->board.overlay.ecost._dst	= base->containers[1].containers[1].containers[0].elems[1].boundingBox;
	ctx->board.overlay.pcost._dst	= base->containers[1].containers[1].containers[0].elems[2].boundingBox;
	ctx->board.overlay.rcost._dst	= base->containers[1].containers[1].containers[1].elems[1].boundingBox;
	ctx->board.overlay.effect._dst	= base->containers[2].elems[0].boundingBox;
	ctx->board.overlay.exit.sprite._dst = root->containers[0].containers[2].containers[0].elems[0].boundingBox;
	ctx->board.overlay.buy.sprite._dst = root->containers[0].containers[2].containers[1].containers[0].elems[0].boundingBox;

	// ctx->board.overlay.name.texture 	= create_target_texture(ctx->board.overlay.name._dst.w, ctx->board.overlay.name._dst.h);
	if (!ok)
	{
		ctx->board.overlay.bg.texture 		= create_target_texture(root->self.boundingBox.w, root->self.boundingBox.h);
		ctx->board.overlay.name.texture 	= create_target_texture(ctx->board.overlay.name._dst.w, ctx->board.overlay.name._dst.h);
		ctx->board.overlay.potion.texture 	= create_target_texture(ctx->board.overlay.potion._dst.w, ctx->board.overlay.potion._dst.h);
		ctx->board.overlay.cost.texture 	= create_target_texture(ctx->board.overlay.cost._dst.w, ctx->board.overlay.cost._dst.h);
		ctx->board.overlay.effect.texture 	= create_target_texture(ctx->board.overlay.effect._dst.w, ctx->board.overlay.effect._dst.h);
		ctx->board.overlay.recycle.texture 	= create_target_texture(ctx->board.overlay.recycle._dst.w, ctx->board.overlay.recycle._dst.h);
		ctx->board.overlay.ecost.texture 	= create_target_texture(ctx->board.overlay.ecost._dst.w, ctx->board.overlay.ecost._dst.h);
		ctx->board.overlay.pcost.texture 	= create_target_texture(ctx->board.overlay.pcost._dst.w, ctx->board.overlay.pcost._dst.h);
		ctx->board.overlay.rcost.texture 	= create_target_texture(ctx->board.overlay.rcost._dst.w, ctx->board.overlay.rcost._dst.h);
		// ctx->board.overlay.exit.sprite.texture 	= create_target_texture(ctx->board.overlay.exit.sprite._dst.w, ctx->board.overlay.exit.sprite._dst.h);
		ctx->board.overlay.buy.sprite.texture 	= create_target_texture(ctx->board.overlay.buy.sprite._dst.w, ctx->board.overlay.buy.sprite._dst.h);

		overlay_texture(ctx->board.overlay.bg.texture, ctx->assets.overlayBG, NULL, NULL);
		overlay_texture(ctx->board.overlay.bg.texture, ctx->assets.recipe, &root->containers[0].containers[1].self.boundingBox, NULL);
		overlay_text(ctx->board.overlay.cost.texture , NULL, NULL, BLACK, 1, "Cost : ");
		overlay_text(ctx->board.overlay.recycle.texture , NULL, NULL, BLACK, 1, "Recycle for : ");
		overlay_text(ctx->board.overlay.buy.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, BLACK, 0.6, "Buy");
	}
	// overlay_texture(ctx->board.overlay.exit.sprite.texture, ctx->assets.texUI,NULL,  &(SDL_Rect){.x = 485, .y = 0, .w = 140, .h = 155});
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
		if (!ok) {

		if (row->card[i].sprite.texture)
			SDL_DestroyTexture(row->card[i].sprite.texture);
		row->card[i].sprite.texture = SDL_CreateTexture(ctx->display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, row->card[i].sprite._dst.w, row->card[i].sprite._dst.h);
		SDL_SetTextureBlendMode(row->card[i].sprite.texture , SDL_BLENDMODE_BLEND);
		}
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
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}
	else
	{
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}
	fillMasterPotions(ctx, &ctx->board.masterPotions, &_root->containers[CARDS].containers[MASTER_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[0], &_root->containers[CARDS].containers[LVL2_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[1], &_root->containers[CARDS].containers[LVL1_ROW]);
	fillRowPotions(ctx, &ctx->board.rows[2], &_root->containers[CARDS].containers[BASE_ROW]);

	fillOverlay(ctx, _overlayRoot);
	// ctx->board.switchScreen.sprite._dst = _root->containers[0].containers[0].containers[1].elems[0].boundingBox;
	ctx->board.switchScreen.sprite._dst = _root->containers[0].containers[1].elems[0].boundingBox;
}
