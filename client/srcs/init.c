#include "../includes/splendor.h"


SDLX_RectContainer *initUI(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}

void initConnectScreen(Context *ctx)
{
	SDL_Rect rect = {
		.x = ctx->display->win_w / 4,
		.y = ctx->display->win_h / 4,
		.w = ctx->display->win_w / 2,
		.h = ctx->display->win_h / 2
	};

	ctx->connection.connectButton._boundingBox = rect;
	SDLX_ButtonCreate(&ctx->connection.connectButton, &ctx->connection.connectButton._boundingBox);
	SDL_Log("Button %d %d",ctx->connection.connectButton.boundingBox->w, ctx->connection.connectButton.boundingBox->h);
}

void initBuyScreen(Context *ctx)
{

}

void initBoard(Context *ctx)
{

}


void init(Context *ctx)
{
	SDLX_RectContainer *root;

	SDLX_Init("Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
	ctx->display = SDLX_DisplayGet();
	// initConnectScreen(ctx);
	init_main_screen(ctx);
	initConnection(ctx);
}
