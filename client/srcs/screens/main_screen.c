#include "../includes/splendor.h"

# define SWITCH_BUTTTON MAIN_SCREEN_SPRITE_COUNT - 2
# define EXTRA_TOKEN MAIN_SCREEN_SPRITE_COUNT - 1

void main_screen(Context *ctx)
{
	if (ctx->UI.switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 1;
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer, 255,0,0,255);
	for (int i = 0; i < CARD_TYPES; i++)
	{
		// SDLX_RenderQueuePush(&ctx->player.reserved[i]);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.tokens[i].dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.permanents[i].dst);
	}
	SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.tokens[TOK_COUNT - 1].dst);
	if (ctx->UI.switchMode.enabled)
		SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.switchMode.boundingBox);
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->UI.reserved[i].state == SDLX_FOCUS_STAY),
					255 * (ctx->UI.reserved[i].triggered == SDLX_KEYHELD),
					255,
					255);
		if (ctx->UI.reserved[i].triggered == SDLX_KEYDOWN)
		{
			ctx->state = 2;
			ctx->buyscreen.cardOrigin = 1;
			ctx->buyscreen.selected = &ctx->player.reserved[i];
			ctx->buyscreen.showSelected.src = ctx->player.reserved[i].sprite.src;
			ctx->buyscreen.showSelected.texture = ctx->player.reserved[i].sprite.texture;
		}
		SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.reserved[i].boundingBox);
		SDLX_RenderQueuePush(&ctx->player.reserved[i].sprite);
		// SDL_SetRenderDrawColor(ctx->display->renderer, 0,255,0,255);
		// SDL_RenderDrawRect(ctx->display->renderer, ctx->player.reserved[i].sprite.dst);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0,0,0,255);
}

