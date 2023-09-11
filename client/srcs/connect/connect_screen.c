#include "../includes/splendor.h"
#include <emscripten.h>

void connect_screen(Context *ctx)
{
	SDLX_Input input;

	input = SDLX_InputGet();
	if (ctx->connection.connectButton.triggered == SDLX_KEYDOWN)
	{
		if (ctx->connection.status == CONNECTED)
			EM_ASM({handleConnect()});
		if (ctx->connection.status == CONNECTEDCONSOLE)
			EM_ASM({handleDisconnect()});
	}
	renderConnectScreen(ctx);
}

void renderConnectScreen(Context *ctx)
{
	SDL_Rect dst;

	dst = *ctx->connection.connectButton.boundingBox;

	dst.w /= 2;
	dst.h /= 3;
	dst.x += dst.w / 2;
	dst.y += dst.h;

	if (ctx->connection.status == CONNECTEDCONSOLE)
	{
		SDLX_RenderMessage(ctx->display, ctx->connection.connectButton.boundingBox, (SDL_Color){0,255,255}, "WAITING");
	}
	else if (ctx->connection.status == CONNECTED)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer, 0,255,0, 255);
		SDL_RenderFillRect(ctx->display->renderer, ctx->connection.connectButton.boundingBox);
		SDLX_RenderMessage(ctx->display, &dst, (SDL_Color){255,255,255,255}, "CONNECT");
	}
	else if (ctx->connection.status == DISCONNECTED)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer, 0,255,0, 255);
		SDLX_RenderMessage(ctx->display, &dst, (SDL_Color){255,0,0,0}, "DISCONNECTED");
	}

	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
