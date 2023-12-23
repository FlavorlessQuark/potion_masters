#include "game.h"

int title_screen(Context *ctx)
{
	if (ctx->connectscreen.counter >= 0)
	{
		if (ctx->connectscreen.counter / MULT <= 255)
		{
			SDL_SetTextureAlphaMod(ctx->display->background, ctx->connectscreen.counter / MULT);
		}
		ctx->connectscreen.counter--;
	}
	else
	{
		// SDL_DestroyTexture(ctx->display->background);
		// ctx->display->background = NULL;
		ctx->state = CONNECT_SCREEN;
	}
	SDL_RenderCopy(ctx->display->renderer, ctx->display->background,  NULL, NULL);
}
