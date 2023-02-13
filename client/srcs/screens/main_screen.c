#include "../includes/splendor.h"

# define SWITCH_BUTTTON MAIN_SCREEN_SPRITE_COUNT - 2
# define EXTRA_TOKEN MAIN_SCREEN_SPRITE_COUNT - 1

void main_screen(Context *ctx)
{
	if (ctx->mainscreen->switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 1;
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255,0,0,255);
	for (int i = 0; i < MAIN_SCREEN_SPRITE_COUNT - (MAX_RESERVE + 2); i++)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[i].dst);
	}
	SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[EXTRA_TOKEN].dst);
	if (ctx->mainscreen->switchMode.enabled)
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->UI[SWITCH_BUTTTON].dst);
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->mainscreen->reserved[i].state == SDLX_FOCUS_STAY),
					255 * (ctx->mainscreen->reserved[i].triggered == SDLX_KEYHELD),
					255,
					255);
		if (ctx->mainscreen->reserved[i].triggered == SDLX_KEYDOWN)
		{
			ctx->state = 2;
			ctx->buyscreen->cardOrigin = 1;
		}
		SDL_RenderDrawRect(ctx->display->renderer, ctx->mainscreen->reserved[i].boundingBox);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0,0,0,255);
}

