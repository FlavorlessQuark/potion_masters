#include "../includes/splendor.h"

void init_potions(Context *ctx)
{
	for (int i = 0; i < MAX_POTIONS; ++i)
	{
		SDLX_SpriteCreate(&ctx->mainUI.owned[i].sprite, 1, NULL);
		SDLX_ButtonCreate(&ctx->mainUI.ownedButtons[i], &ctx->mainUI.owned[i].sprite._dst);
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		SDLX_SpriteCreate(&ctx->mainUI.essences[i], 1, NULL);
	}
}

void init_main_screen(Context *ctx)
{
	init_potions(ctx);

	SDLX_SpriteCreate(&ctx->mainUI.endTurn.sprite, 1, NULL);
	SDLX_ButtonCreate(&ctx->mainUI.endTurn.button, &ctx->mainUI.endTurn.sprite._dst);

	SDLX_SpriteCreate(&ctx->mainUI.switchScreen.sprite, 1, NULL);
	SDLX_ButtonCreate(&ctx->mainUI.switchScreen.button, &ctx->mainUI.switchScreen.sprite._dst);

	// SDL_Log();
	SDL_Log("-0 end)Should be %p, is %p", &ctx->mainUI.endTurn.sprite._dst, ctx->mainUI.endTurn.button.boundingBox);
	SDL_Log("=0 switch) Should be %p, is %p", &ctx->mainUI.switchScreen.sprite._dst, ctx->mainUI.switchScreen.button.boundingBox);

	// endTurn(ctx);
}



