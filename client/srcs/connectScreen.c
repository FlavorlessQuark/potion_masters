#include "../includes/splendor.h"

void ConnectScreen(Context *ctx)
{
	if (ctx->connection.status == HASMESSAGE)
	{
		parseMsg(ctx, ctx->connection.message);
		ctx->connection.status = CONNECTED;
	}
	renderConnectScreen(ctx);
}

void renderConnectScreen(Context *ctx)
{
	SDL_Rect rect = {
		.x = ctx->display->win_w / 4,
		.y = ctx->display->win_h / 4,
		.w = ctx->display->win_w / 2,
		.h = ctx->display->win_h / 2
	};
	SDL_SetRenderDrawColor(ctx->display->renderer,
		(ctx->connection.status == DISCONNECTED) * 255,
		(ctx->connection.status == CONNECTED) * 255,
		(ctx->connection.status == HASMESSAGE) * 255,
		255
		);
	SDL_RenderDrawRect(ctx->display->renderer, &rect);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
