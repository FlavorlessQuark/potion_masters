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

# define OFFSET 40
# define WAIT_TEXT_W  1120
# define WAIT_TEXT_H  160
# define CONNECT_TEXT_W  850
# define CONNECT_TEXT_H  120
# define BUTTONS_W  700
# define BUTTONS_H  240
# define WAIT_TIME 25
void renderConnectScreen(Context *ctx)
{
	static int idx;
	static int timer = WAIT_TIME;
	char *toDisplay = NULL;

	if (ctx->connection.status == READY)
	{
		ctx->connection.text._src.y =  (OFFSET) + (WAIT_TEXT_H + OFFSET) * 4;
		ctx->connection.connectSprite._src.y =  OFFSET * 2 + BUTTONS_H;
		SDLX_RenderQueuePush(&ctx->connection.text);
		SDLX_RenderQueuePush(&ctx->connection.connectSprite);
	}
	else if (ctx->connection.status == CONNECTED)
	{
		ctx->connection.connectSprite._src.y = OFFSET;
		SDLX_RenderQueuePush(&ctx->connection.connectSprite);
	}
	else if (ctx->connection.status == DISCONNECTED)
	{

		ctx->connection.text._src.y = (OFFSET) + (WAIT_TEXT_H + OFFSET) * idx;
		if (timer == 0)
		{
			idx = (idx + 1) % 4;
			timer = WAIT_TIME;
		}
		SDLX_RenderQueuePush(&ctx->connection.text);
	}
	timer--;
}
