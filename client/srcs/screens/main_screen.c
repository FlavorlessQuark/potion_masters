#include "../includes/splendor.h"

# define SWITCH_BUTTTON MAIN_SCREEN_SPRITE_COUNT - 2
# define EXTRA_TOKEN MAIN_SCREEN_SPRITE_COUNT - 1

void main_screen(Context *ctx)
{
	if (ctx->switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = CARDSELECT;
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
	ctx->UI.points._src.x = ctx->nums.x + (ctx->player.points * ctx->nums.w);
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
	if (ctx->switchMode.enabled == SDL_TRUE)
	{
		SDL_Log("aaaaa %d", ctx->switchMode.triggered);
		SDLX_RenderQueuePush(&ctx->switchSprite);
	}

	for (int i = 0; i < ctx->player.reserveCount; i++)
		SDLX_RenderQueuePush(&ctx->player.reserved[i].sprite);
}

