#include "../includes/splendor.h"


void init_potions(Context *ctx)
{

	for (int i = 0; i < MAX_POTIONS ; ++i)
	{
			SDLX_SpriteCreate(&ctx->mainUI.owned[i].sprite, 1, NULL);
			SDLX_ButtonCreate(&ctx->mainUI.ownedButtons[i], &ctx->mainUI.owned[i].sprite._dst);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->mainUI.essences[i],1, NULL);
	}
}

void init_overlay(Context *ctx)
{
	SDLX_SpriteCreate(&ctx->mainUI.overlay.use.sprite, 1, NULL);
	SDLX_SpriteCreate(&ctx->mainUI.overlay.exit.sprite, 1, NULL);
	SDLX_SpriteCreate(&ctx->mainUI.overlay.convert.sprite, 1, NULL);

	SDLX_ButtonCreate(&ctx->mainUI.overlay.use.button, &ctx->mainUI.overlay.use.sprite._dst);
	SDLX_ButtonCreate(&ctx->mainUI.overlay.exit.button, &ctx->mainUI.overlay.exit.sprite._dst);
	SDLX_ButtonCreate(&ctx->mainUI.overlay.convert.button, &ctx->mainUI.overlay.convert.sprite._dst);


	SDLX_SpriteCreate(&ctx->mainUI.overlay.name, 1, NULL);
	SDLX_SpriteCreate(&ctx->mainUI.overlay.potion, 1, NULL);
	SDLX_SpriteCreate(&ctx->mainUI.overlay.effect, 1, NULL);
	SDLX_SpriteCreate(&ctx->mainUI.overlay.fillAmount, 1, NULL);
}

void init_main_screen(Context *ctx)
{
	init_potions(ctx);
	init_overlay(ctx);
	SDLX_SpriteCreate(&ctx->mainUI.switchScreen.sprite, 1, NULL);
	SDLX_ButtonCreate(&ctx->mainUI.switchScreen.button, &ctx->mainUI.switchScreen.sprite._dst);

	SDLX_SpriteCreate(&ctx->mainUI.endTurn.sprite, 1, NULL);
	SDLX_ButtonCreate(&ctx->mainUI.endTurn.button, &ctx->mainUI.endTurn.sprite._dst);

}



