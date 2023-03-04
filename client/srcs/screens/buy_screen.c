#include "../includes/splendor.h"

void buy_screen(Context *ctx)
{
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
		sendPay(ctx);
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

	SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->buyscreen.buyButton.triggered == SDLX_KEYHELD),
					255 * (ctx->buyscreen.buyButton.state == SDLX_FOCUS_STAY),
					255,
					255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen.buyButton.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->buyscreen.reserveButton.triggered == SDLX_KEYHELD),
					255 * (ctx->buyscreen.reserveButton.state == SDLX_FOCUS_STAY),
					255,
					255);
	if (ctx->buyscreen.cardOrigin == 0 && ctx->player.reserveCount < MAX_RESERVE)
		SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen.reserveButton.boundingBox);

	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 255, 255);

	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen.showSelected.dst);
	SDLX_RenderQueuePush(&ctx->buyscreen.showSelected);
	SDL_SetRenderDrawColor(ctx->display->renderer,
					255 * (ctx->buyscreen.exit.triggered == SDLX_KEYHELD),
					255 ,
					0,
					255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->buyscreen.exit.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

