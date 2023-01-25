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
	SDLX_RectContainer *root;

	root = initUI("assets/UI/connectUI");
}

void initBuyScreen(Context *ctx)
{

}

void initBoard(Context *ctx)
{

}

void initPlayerScreen(Context *ctx)
{

}

void init(Context *ctx)
{
	SDLX_RectContainer *root;

	SDLX_InitDefault();
	ctx->display = SDLX_DisplayGet();
	initConnection(ctx);
}
