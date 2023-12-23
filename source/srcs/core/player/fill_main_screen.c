#include "../includes/game.h"

#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define POTION_ROWS 3

static SDLX_RectContainer *_root;
static SDLX_RectContainer *_overlayRoot;

void fill_potions(Context *ctx, SDLX_RectContainer *root)
{
	SDL_Rect src = {.x = 0, .y = 35, .w = 897, .h = 145};
	SDL_Rect dst;


	overlay_texture(ctx->mainUI.bg, ctx->assets.main_bg, NULL, NULL);
	for (int i = 0; i < MAX_POTIONS / 3; ++i)
	{
		// overlay_text(ctx->mainUI.bg.texture, ctx->assets.main_bg, NULL, 0xFFFFFFFF, 1, "PLEASE D:");
		dst = root->containers[0].containers[i].containers[1].elems[0].boundingBox;
		dst.h += dst.h / 2;
		overlay_texture(ctx->mainUI.bg, ctx->assets.shelf, &dst, &src);
		src.y += src.h + 42;
		for (int j = 0; j < MAX_POTIONS / 3; ++j)
		{
			ctx->player.owned[i * 3 + j].sprite._dst = root->containers[0].containers[i].containers[0].elems[j].boundingBox;
			ctx->player.owned[i * 3 + j].sprite.texture = create_target_texture(ctx->player.owned[i * 3 + j].sprite._dst.w, ctx->player.owned[i * 3 + j].sprite._dst.h);
			ctx->player.owned[i * 3 + j].sprite.src = NULL;
		}
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		ctx->mainUI.essences[i]._dst = root->containers[1].containers[0].containers[1].elems[i].boundingBox;
		ctx->mainUI.essences[i].texture = create_target_texture(ctx->mainUI.essences[i]._dst.w, ctx->mainUI.essences[i]._dst.h);
	}
	ctx->player.brewing.sprite._dst =  root->containers[1].containers[0].containers[0].containers[0].elems[0].boundingBox;
	ctx->player.brewing.sprite.texture = create_target_texture(ctx->player.brewing.sprite._dst.w, ctx->player.brewing.sprite._dst.h);
}
void fill_main_overlay(Context *ctx, SDLX_RectContainer *root)
{
	SDLX_RectContainer *base;

	base = &root->containers[0].containers[1].containers[0].containers[0];

	ctx->mainUI.overlay.fillAmount._dst = 	base->containers[0].containers[0].elems[0].boundingBox;
	ctx->mainUI.overlay.potion._dst = 		base->containers[1].containers[0].containers[0].self.boundingBox;
	ctx->mainUI.overlay.name._dst = 		base->containers[1].containers[0].containers[1].self.boundingBox;
	ctx->mainUI.overlay.exit.sprite._dst = 	base->containers[0].containers[1].elems[0].boundingBox;


	ctx->mainUI.overlay.use.sprite._dst =	base->containers[1].containers[1].containers[0].containers[0].elems[0].boundingBox;
	ctx->mainUI.overlay.effect._dst =		base->containers[1].containers[1].containers[0].containers[0].elems[1].boundingBox;

	ctx->mainUI.overlay.convert.sprite._dst = base->containers[1].containers[1].containers[1].containers[0].elems[0].boundingBox;
	ctx->mainUI.overlay.recyle._dst = 		  base->containers[1].containers[1].containers[1].containers[0].elems[1].boundingBox;

	ctx->mainUI.overlay.fillAmount.texture = 	create_target_texture(ctx->mainUI.overlay.fillAmount._dst.w, ctx->mainUI.overlay.fillAmount._dst.h);
	// ctx->mainUI.overlay.potion.texture = 		create_target_texture(ctx->mainUI.overlay.potion._dst.w, 	 ctx->mainUI.overlay.potion._dst.h);
	// ctx->mainUI.overlay.exit.sprite.texture = 	create_target_texture(ctx->mainUI.overlay.exit.sprite._dst.w,ctx->mainUI.overlay.exit.sprite._dst.h);
	ctx->mainUI.overlay.name.texture = 			create_target_texture(ctx->mainUI.overlay.name._dst.w, 		 ctx->mainUI.overlay.name._dst.h);
	ctx->mainUI.overlay.use.sprite.texture = 	create_target_texture(ctx->mainUI.overlay.use.sprite._dst.w, ctx->mainUI.overlay.use.sprite._dst.h);
	ctx->mainUI.overlay.effect.texture = 		create_target_texture(ctx->mainUI.overlay.effect._dst.w, 	 ctx->mainUI.overlay.effect._dst.h);
	ctx->mainUI.overlay.convert.sprite.texture= create_target_texture(ctx->mainUI.overlay.convert.sprite._dst.w, ctx->mainUI.overlay.convert.sprite._dst.h);
	ctx->mainUI.overlay.recyle.texture = 		create_target_texture(ctx->mainUI.overlay.recyle._dst.w, 	 ctx->mainUI.overlay.recyle._dst.h);

	overlay_text(ctx->mainUI.overlay.convert.sprite.texture, ctx->assets.texUI,  &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, BLACK, 0.7, "CONVERT");
	overlay_text(ctx->mainUI.overlay.use.sprite.texture, ctx->assets.texUI,  &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, BLACK, 0.7, "USE");

}


void fill_main_screen(Context *ctx)
{

	if (!_root)
	{
		_root = SDLX_LoadConfig("assets/UI/playerUI");
		_overlayRoot = SDLX_LoadConfig("assets/UI/potionUI");
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}
	else
	{
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}

	ctx->mainUI.bg = create_target_texture(ctx->display->win_w, ctx->display->win_h);
	fill_potions(ctx, _root);
	fill_main_overlay(ctx, _overlayRoot);


	// ctx->mainUI.bg.texture = ctx->assets.main_bg;


	ctx->mainUI.endTurn.sprite._dst = 		_root->containers[1].containers[1].containers[0].elems[1].boundingBox;
	ctx->mainUI.endTurn.sprite.texture = create_target_texture(ctx->mainUI.endTurn.sprite._dst.w, ctx->mainUI.endTurn.sprite._dst.h);
	overlay_text(ctx->mainUI.endTurn.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180},  BLACK, 0.6, "END turn");

	ctx->mainUI.actions.texture = create_target_texture(ctx->mainUI.endTurn.sprite._dst.w, ctx->mainUI.endTurn.sprite._dst.h);
	ctx->mainUI.actions._dst = _root->containers[1].containers[1].containers[0].elems[0].boundingBox;

	ctx->mainUI.switchScreen.sprite._dst = 	_root->containers[1].containers[1].containers[1].elems[0].boundingBox;


	ctx->mainUI.actions.src = NULL;
}
