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
void fill_main_overlay(Context *ctx, SDLX_RectContainer *root)
{
	ctx->mainUI.overlay.fillAmount._dst = root->containers[0].containers[0].elems[0].boundingBox;
	ctx->mainUI.overlay.potion._dst = root->containers[0].containers[1].elems[0].boundingBox;
	ctx->mainUI.overlay.name._dst = root->containers[0].containers[2].elems[0].boundingBox;

	ctx->mainUI.overlay.use.sprite._dst = root->containers[1].containers[2].elems[0].boundingBox;
	ctx->mainUI.overlay.exit.sprite._dst = root->containers[1].containers[0].elems[0].boundingBox;
	ctx->mainUI.overlay.convert.sprite._dst = root->containers[1].containers[3].elems[0].boundingBox;

	ctx->mainUI.overlay.effect._dst = root->containers[1].containers[1].elems[0].boundingBox;
}


void fill_main_screen(Context *ctx)
{

	if (!_root)
	{
		_root = SDLX_LoadConfig("assets/UI/playerUI");
		_overlayRoot = SDLX_LoadConfig("assets/UI/potionUI");
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 4;
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}
	else
	{
		_overlayRoot->self.boundingBox.x = SDLX_DisplayGet()->win_w / 3;
		_overlayRoot->self.boundingBox.y = SDLX_DisplayGet()->win_h / 4;
		SDLX_ContainerUpdate(_root, NULL);
		SDLX_ContainerUpdate(_overlayRoot, NULL);
	}

	fill_potions(ctx, _root);
	fill_main_overlay(ctx, _overlayRoot);

	ctx->mainUI.endTurn.sprite._dst = _root->containers[1].containers[1].elems[0].boundingBox;
	ctx->mainUI.switchScreen.sprite._dst = _root->containers[1].containers[1].elems[1].boundingBox;
}
