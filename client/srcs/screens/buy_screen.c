#include "../includes/splendor.h"

int canBuy(Context *ctx)
{
	int missing;

	missing = 0;

	for (int i = 0; i < CARD_TYPES; i++)
	{
		if (ctx->player.owned[i] + ctx->player.tokens[i] < ctx->buyscreen.selected->cost[i])
			missing += ctx->buyscreen.selected->cost[i] -( ctx->player.owned[i] + ctx->player.tokens[i]);
	}
	if (missing <= ctx->player.tokens[CARD_TYPES])
		return 1;
	return 0;
}

void buy_screen(Context *ctx)
{
	SDL_Log("??? %d %d %d | %d", ctx->buyscreen.cardOrigin == 1, ctx->player.reserveCount < MAX_RESERVE, ctx->board.tokens[CARD_TYPES] > 0, ctx->buyscreen.cardOrigin == 1 && ctx->player.reserveCount < MAX_RESERVE && ctx->board.tokens[CARD_TYPES] > 0);
	if (ctx->buyscreen.cardOrigin == 0 && ctx->player.reserveCount < MAX_RESERVE && ctx->board.tokens[CARD_TYPES] > 0)
		ctx->buyscreen.reserveButton.enabled = SDL_TRUE;
	else
		ctx->buyscreen.reserveButton.enabled = SDL_FALSE;

	if (!canBuy(ctx))
		ctx->buyscreen.buyButton.enabled = SDL_FALSE;
	else
		ctx->buyscreen.buyButton.enabled = SDL_TRUE;

	if (ctx->buyscreen.exit.triggered == SDLX_KEYDOWN)
	{
		if (ctx->buyscreen.cardOrigin == 0)
			ctx->state = 1;
		else if (ctx->buyscreen.cardOrigin == 1)
			ctx->state = 0;
		ctx->buyscreen.cardOrigin = -1;
	}
	else if (ctx->buyscreen.buyButton.triggered == SDLX_KEYDOWN)
	{
		for (int i = 0; i < CARD_TYPES; i++)
		{
			if ((ctx->player.tokens[i] + ctx->player.owned[i]) < ctx->buyscreen.selected->cost[i])
				ctx->player.tokens[CARD_TYPES] -= ctx->buyscreen.selected->cost[i] - (ctx->player.tokens[i] + ctx->player.owned[i]);
			ctx->player.tokens[i] -= ctx->buyscreen.selected->cost[i] - ctx->player.owned[i];
		}
		sendPay(ctx);
		ctx->player.owned[ctx->buyscreen.selected->type]++;
		ctx->state = 0;
		ctx->buyscreen.cardOrigin = -1;
	}
	else if (ctx->buyscreen.reserveButton.triggered == SDLX_KEYDOWN)
	{
		sendReserve(ctx);
		ctx->state = 0;
		ctx->buyscreen.cardOrigin = -1;
	}
	render_buy_screen(ctx);
}

void render_buy_screen(Context *ctx)
{
	if (ctx->buyscreen.cardOrigin == 0 && ctx->player.reserveCount < MAX_RESERVE  && ctx->board.tokens[CARD_TYPES] > 0)
	{
		SDLX_RenderQueuePush(&ctx->buyscreen.reserveSprite);
	}
	if (canBuy(ctx))
	{
		SDLX_RenderQueuePush(&ctx->buyscreen.buySprite);
	}

	SDL_RenderCopy(ctx->display->renderer, ctx->buyscreen.bg, NULL, NULL);
	SDLX_RenderQueuePush(&ctx->buyscreen.showSelected);

	ctx->buyscreen.costSprite[0]._src.x = ctx->nums.x + (ctx->buyscreen.selected->cost[0] * ctx->nums.w);
	ctx->buyscreen.costSprite[1]._src.x = ctx->nums.x + (ctx->buyscreen.selected->cost[1] * ctx->nums.w);
	ctx->buyscreen.costSprite[2]._src.x = ctx->nums.x + (ctx->buyscreen.selected->cost[2] * ctx->nums.w);
	ctx->buyscreen.costSprite[3]._src.x = ctx->nums.x + (ctx->buyscreen.selected->cost[3] * ctx->nums.w);
	SDLX_RenderQueuePush(&ctx->buyscreen.costSprite[0]);
	SDLX_RenderQueuePush(&ctx->buyscreen.costSprite[1]);
	SDLX_RenderQueuePush(&ctx->buyscreen.costSprite[2]);
	SDLX_RenderQueuePush(&ctx->buyscreen.costSprite[3]);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen.exit.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

