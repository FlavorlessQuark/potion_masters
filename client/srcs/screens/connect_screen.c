#include "../includes/splendor.h"
#include <emscripten.h>

void connect_screen(Context *ctx)
{
	SDLX_Input input;

	input = SDLX_InputGet();
	if (ctx->connection.connectButton.triggered == SDLX_KEYDOWN)
	{
		if (ctx->connection.status == CONNECTED)
			ctx->connection.status = READY;
		else if (ctx->connection.status == READY)
			ctx->connection.status = CONNECTED;
		sendStatus(ctx);
	}
	renderConnectScreen(ctx);
}

# define SCALAR 0.58
# define OFFSET 40 * SCALAR
# define WAIT_TEXT_W  1120 * SCALAR
# define WAIT_TEXT_H  160 * SCALAR
# define CONNECT_TEXT_W  850 * SCALAR
# define CONNECT_TEXT_H  120 * SCALAR
# define BUTTONS_W  700 * SCALAR
# define BUTTONS_H  240 * SCALAR
# define WAIT_TIME 25
void renderConnectScreen(Context *ctx)
{
	static int idx;
	static int timer = WAIT_TIME;
	char *toDisplay = NULL;

	if (ctx->connection.status == READY)
	{
		SDLX_RenderMessage(ctx->display, ctx->connection.connectButton.boundingBox, (SDL_Color){0,255,255}, "WAITING");
	}
	else if (ctx->connection.status == CONNECTED)
	{

		SDL_RenderFillRect(ctx->display->renderer, ctx->connection.connectButton.boundingBox);

	}
	else if (ctx->connection.status == DISCONNECTED)
	{
		ctx->connection.text._src.y = (OFFSET) + (WAIT_TEXT_H + OFFSET) * idx;
		if (timer == 0)
		{
			idx = (idx + 1) % 4;
			timer = WAIT_TIME;
		}
	}
	timer--;
	// SDLX_RenderMessage(ctx->display, &dst, color, toDisplay);
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(ctx->display->renderer, ctx->connection.connectButton.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(ctx->display->renderer, ctx->connection.connectSprite.dst);
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(ctx->display->renderer, ctx->connection.text.dst);

	SDLX_RenderQueuePush(&ctx->connection.text);
	SDLX_RenderQueuePush(&ctx->connection.connectSprite);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
