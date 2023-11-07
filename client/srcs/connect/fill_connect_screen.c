#include "splendor.h"

static SDLX_RectContainer *_root = NULL;

void fill_connect_screen(Context *ctx)
{
	if (!_root)
	{
		_root = SDLX_LoadConfig("assets/UI/connectUI");
	}
	else
	{
		SDLX_ContainerUpdate(_root, NULL);
	}

	ctx->connection.bg._dst = _root->containers[0].containers[0].self.boundingBox;
	ctx->connection.bg._dst.w = _root->containers[0].containers[0].self.boundingBox.h;
	ctx->connection.bg._dst.h = _root->containers[0].containers[0].self.boundingBox.w;
	ctx->connection.bg._dst.x += ctx->connection.bg._dst.h ;
	ctx->connection.bg._dst.y -= 40;


	ctx->connection.name._dst =  _root->containers[0].containers[0].self.boundingBox;
	ctx->connection.connectSprite._dst = _root->containers[0].containers[0].elems[1].boundingBox;

	ctx->connection.connectSprite._dst = scale_and_center(0.4, ctx->connection.connectSprite._dst, ctx->connection.connectSprite._dst);
	ctx->connection.connectSprite._dst.y += 60;

	ctx->connection.bg._src = (SDL_Rect){.x = 0, .y = 0, .w = 470, .h = 470};
	ctx->connection.bg.angle = 90.0;
	ctx->connection.bg.center = &ctx->connection.bg._center;
	ctx->connection.bg._center = (SDL_Point){.x = 0, .y = 0};

	ctx->connection.connectSprite.src = NULL;
	ctx->connection.name.src = NULL;
	SDL_Rect src = {.x = 0, .y = 470, .w = 310, .h = 180};

	overlay_text(ctx->connection.connectSprite.texture, ctx->assets.texUI, &src, BLACK, 0.6, "Ready");
	ctx->connection.name._dst.y -= 50;


}
