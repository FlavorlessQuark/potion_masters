#include "../includes/splendor.h"

#define LEFT 0
#define CENTER 1
#define RIGHT 2
#define POTION_ROWS 3

static SDLX_RectContainer *_root;
static SDLX_RectContainer *_overlayRoot;

void fill_potions(Context *ctx, SDLX_RectContainer *root)
{
	for (int i = 0; i < MAX_POTIONS / 3; ++i)
	{
		for (int j = 0; j < MAX_POTIONS / 3; ++j)
		{
			ctx->mainUI.owned[i * 3 + j].sprite._dst = root->containers[0].containers[i].containers[0].elems[j].boundingBox;
		}
	}
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		ctx->mainUI.essences[i]._dst = root->containers[1].containers[0].containers[0].elems[i].boundingBox;
	}
}


void fill_main_screen(Context *ctx)
{

	if (!_root)
	{
		_root = SDLX_LoadConfig("assets/UI/playerUI");
		_overlayRoot = SDLX_LoadConfig("assets/UI/potionUI");
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 3;
	}
	else
	{
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 4;
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}

	fill_potions(ctx, _root);

	ctx->mainUI.endTurn.sprite._dst = _root->containers[1].containers[1].elems[0].boundingBox;
	ctx->mainUI.switchScreen.sprite._dst = _root->containers[1].containers[1].elems[1].boundingBox;

	SDL_Log("end)Should be %p, is %p", &ctx->mainUI.endTurn.sprite._dst, ctx->mainUI.endTurn.button.boundingBox);
	SDL_Log("switch) Should be %p, is %p", &ctx->mainUI.switchScreen.sprite._dst, ctx->mainUI.switchScreen.button.boundingBox);

}
