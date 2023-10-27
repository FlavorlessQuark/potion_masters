#include "../includes/splendor.h"


void init_potions(Context *ctx)
{
	SDLX_SpriteCreate(&ctx->player.brewing.sprite, 1,  create_target_texture(500, 500));
	ctx->player.brewing.sprite.src = NULL;
	for (int i = 0; i < MAX_POTIONS ; ++i)
	{
			ctx->player.owned->fill = 0;
			SDLX_SpriteCreate(&ctx->player.owned[i].sprite, 1,  create_target_texture(500, 500));
			SDLX_ButtonCreate(&ctx->mainUI.ownedButtons[i], &ctx->player.owned[i].sprite._dst);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->mainUI.essences[i],1,  create_target_texture(500, 500));
		ctx->mainUI.essences[i].src = NULL;
	}
}

void init_overlay(Context *ctx)
{
	SDLX_SpriteCreate(&ctx->mainUI.overlay.use.sprite, 1,  create_target_texture(500, 500));
	SDLX_SpriteCreate(&ctx->mainUI.overlay.exit.sprite, 1,  ctx->assets.texUI);
	SDLX_SpriteCreate(&ctx->mainUI.overlay.convert.sprite, 1,  create_target_texture(200, 100));

	SDLX_ButtonCreate(&ctx->mainUI.overlay.use.button, &ctx->mainUI.overlay.use.sprite._dst);
	SDLX_ButtonCreate(&ctx->mainUI.overlay.exit.button, &ctx->mainUI.overlay.exit.sprite._dst);
	SDLX_ButtonCreate(&ctx->mainUI.overlay.convert.button, &ctx->mainUI.overlay.convert.sprite._dst);

	overlay_text(ctx->mainUI.overlay.use.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, BLACK, 0.6, "Use");
	overlay_text(ctx->mainUI.overlay.convert.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180}, BLACK, 0.6, "Convert");
	ctx->mainUI.overlay.exit.sprite._src = (SDL_Rect){.x = 490, .y = 0, .w = 140, .h = 155};
	ctx->mainUI.overlay.use.sprite.src = NULL;
	ctx->mainUI.overlay.convert.sprite.src = NULL;


	SDLX_SpriteCreate(&ctx->mainUI.overlay.name, 1, create_target_texture(200, 100));
	SDLX_SpriteCreate(&ctx->mainUI.overlay.potion, 1, create_target_texture(500, 500));
	SDLX_SpriteCreate(&ctx->mainUI.overlay.effect, 1, create_target_texture(200, 100));
	SDLX_SpriteCreate(&ctx->mainUI.overlay.fillAmount, 1, create_target_texture(200, 100));


	overlay_text(ctx->mainUI.overlay.effect.texture, NULL, NULL, WHITE, 0.6, "Effect: None");
	ctx->mainUI.overlay.name.src = NULL;
	ctx->mainUI.overlay.effect.src = NULL;
	ctx->mainUI.overlay.fillAmount.src = NULL;
}

void init_main_screen(Context *ctx)
{
	init_potions(ctx);
	init_overlay(ctx);
	SDLX_SpriteCreate(&ctx->mainUI.switchScreen.sprite, 1, ctx->assets.texUI);
	SDLX_ButtonCreate(&ctx->mainUI.switchScreen.button, &ctx->mainUI.switchScreen.sprite._dst);
	ctx->mainUI.switchScreen.sprite._src = (SDL_Rect){.x = 640, .y = 0, .w = 140, .h = 155};

	SDLX_SpriteCreate(&ctx->mainUI.endTurn.sprite, 1,  create_target_texture(500, 500));
	SDLX_ButtonCreate(&ctx->mainUI.endTurn.button, &ctx->mainUI.endTurn.sprite._dst);
	overlay_text(ctx->mainUI.endTurn.sprite.texture, ctx->assets.texUI, &(SDL_Rect){.x = 0, .y = 470, .w = 310, .h = 180},  BLACK, 0.6, "End turn");
	ctx->mainUI.endTurn.sprite.src = NULL;
}



