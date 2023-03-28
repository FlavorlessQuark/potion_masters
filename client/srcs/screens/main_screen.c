#include "../includes/splendor.h"

# define SWITCH_BUTTTON MAIN_SCREEN_SPRITE_COUNT - 2
# define EXTRA_TOKEN MAIN_SCREEN_SPRITE_COUNT - 1

void main_screen(Context *ctx)
{
	if (ctx->UI.switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 1;
	for (int i = 0; i < ctx->player.reserveCount; i++)
	{

		if (ctx->UI.reserved[i].triggered == SDLX_KEYDOWN)
		{
			ctx->state = 2;
			ctx->buyscreen.cardOrigin = 1;
			ctx->buyscreen.selected = &ctx->player.reserved[i];
			ctx->buyscreen.showSelected.src = ctx->player.reserved[i].sprite.src;
			ctx->buyscreen.showSelected.texture = ctx->player.reserved[i].sprite.texture;
		}

		SDLX_RenderQueuePush(&ctx->player.reserved[i].sprite);
	}
	render_main_screen(ctx);
}

void render_main_screen(Context *ctx)
{
	SDL_RenderCopy(ctx->display->renderer, ctx->UI.bg, NULL, NULL);
	SDL_SetRenderDrawColor(ctx->display->renderer, 255,0,0,255);
	ctx->UI.points._src.x = ctx->nums.x + (ctx->player.points * ctx->nums.w);
	SDL_Log("Plauyer oints %d", ctx->player.points);
	SDLX_RenderQueuePush(&ctx->UI.points);
	for (int i = 0; i < CARD_TYPES; i++)
	{
		// SDLX_RenderQueuePush(&ctx->player.reserved[i]);
		ctx->UI.tokens[i]._src.x = ctx->nums.x + (ctx->player.tokens[i] * ctx->nums.w);
		ctx->UI.permanents[i]._src.x = ctx->nums.x + (ctx->player.owned[i] * ctx->nums.w);
		SDLX_RenderQueuePush(&ctx->UI.tokens[i]);
		SDLX_RenderQueuePush(&ctx->UI.permanents[i]);
	}
	ctx->UI.tokens[CARD_TYPES]._src.x = ctx->nums.x + (ctx->player.tokens[CARD_TYPES] * ctx->nums.w);
	SDLX_RenderQueuePush(&ctx->UI.tokens[CARD_TYPES]);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.tokens[TOK_COUNT - 1].dst);
	if (ctx->UI.switchMode.enabled)
		SDL_RenderDrawRect(ctx->display->renderer, ctx->UI.switchMode.boundingBox);
	for (int i = 0; i < ctx->player.reserveCount; i++)
		SDLX_RenderQueuePush(&ctx->player.reserved[i].sprite);

	SDL_SetRenderDrawColor(ctx->display->renderer, 0,0,0,255);
}

