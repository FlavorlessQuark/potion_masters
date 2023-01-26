#include "../includes/splendor.h"
#include <emscripten.h>

void ConnectScreen(Context *ctx)
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

	if (ctx->connection.hasMessage == SDL_TRUE)
	{
		parseMsg(ctx, ctx->connection.message);
	}
	renderConnectScreen(ctx);
}

void renderConnectScreen(Context *ctx)
{
	SDL_SetRenderDrawColor(ctx->display->renderer,
		(ctx->connection.status == DISCONNECTED) * 255,
		(ctx->connection.status == CONNECTEDCONSOLE) * 255,
		(ctx->connection.status == CONNECTED) * 255,
		255
		);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->connection.connectButton.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
