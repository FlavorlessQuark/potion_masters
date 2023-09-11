#include "../includes/splendor.h"

void init_connect_screen(Context *ctx)
{
	SDL_Rect rect = {
		.x = ctx->display->win_w / 5,
		.y = ctx->display->win_h / 5,
		.w = ctx->display->win_w / 2,
		.h = ctx->display->win_h / 2
	};

	ctx->connection.connectButton._boundingBox = rect;
	SDLX_ButtonCreate(&ctx->connection.connectButton, &ctx->connection.connectButton._boundingBox);
}

