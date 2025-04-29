#include "game.h"

static SDLX_RectContainer *_root = NULL;

void init_main_menu(Context *ctx)
{
    SDLX_SpriteCreate(&ctx->mainMenu.hostButton.sprite, 1, NULL);
    SDLX_SpriteCreate(&ctx->mainMenu.joinButton.sprite, 1, NULL);

    SDLX_ButtonCreate(&ctx->mainMenu.hostButton.button, &ctx->mainMenu.hostButton.sprite._dst);
    SDLX_ButtonCreate(&ctx->mainMenu.joinButton.button, &ctx->mainMenu.joinButton.sprite._dst);
}


void fill_main_menu(Context *ctx)
{
    if (!_root)
	{
		_root = SDLX_LoadConfig("assets/UI/boardUI");
		SDLX_ContainerUpdate(_root, NULL);
	}
    ctx->mainMenu.joinButton.sprite._dst = _root->containers[1].containers[1].elems[0].boundingBox;
    ctx->mainMenu.hostButton.sprite._dst = _root->containers[1].containers[2].elems[0].boundingBox;
}
