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
		if (ctx->connection.status == READY)
			EM_ASM({handleDisconnect()});
	}
	renderConnectScreen(ctx);
}

void renderConnectScreen(Context *ctx)
{
	SDLX_RenderQueuePush(&ctx->connection.bg);
	SDLX_RenderQueuePush(&ctx->connection.name);
	SDLX_RenderQueuePush(&ctx->connection.connectSprite);
}
