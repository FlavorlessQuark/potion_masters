#include "../includes/splendor.h"

# define OFFSET 40 * 0.58
# define WAIT_TEXT_W  1120
# define WAIT_TEXT_H  160
# define CONNECT_TEXT_W  850
# define CONNECT_TEXT_H  120
# define BUTTONS_W  700
# define BUTTONS_H  240
# define TEXT_RATIO ((double)WAIT_TEXT_H / (double)WAIT_TEXT_W)
# define BUTTON_RATIO ((double)BUTTONS_H / (double)BUTTONS_W)

void init_connect_screen(Context *ctx)
{
	SDL_Rect rect = {
		.x = 0,
		.y = 0,
		.w = ctx->display->win_w,
		.h = ctx->display->win_h
	};
	SDLX_ButtonCreate(&ctx->connection.connectButton, NULL);
	SDLX_SpriteCreate(&ctx->connection.text, 1, ctx->buttonTex);
	SDLX_SpriteCreate(&ctx->connection.connectSprite, 1, ctx->buttonTex);
	ctx->connection.connectButton._boundingBox = scaleAndCenter(0.75, rect , rect);

	ctx->connection.text._dst =  ctx->connection.connectButton._boundingBox;
	ctx->connection.text._dst.h = ctx->connection.text._dst.w * TEXT_RATIO;

	ctx->connection.connectButton.boundingBox->h *= BUTTON_RATIO;
	ctx->connection.connectSprite.dst = ctx->connection.connectButton.boundingBox;

	ctx->connection.connectButton._boundingBox = scaleAndCenter(1, rect , ctx->connection.connectButton._boundingBox);
	ctx->connection.text._dst = scaleAndCenter(1, rect , ctx->connection.text._dst);
	ctx->connection.text._dst.y = ctx->connection.connectSprite.dst->y - ctx->connection.text._dst.h;
	ctx->connection.text._src = (SDL_Rect){.x = 0, .y = OFFSET, .w = WAIT_TEXT_W * 0.58, .h = WAIT_TEXT_H * 0.58};

	ctx->connection.connectSprite._src = (SDL_Rect){.x =  WAIT_TEXT_W * 0.58 + 70, .y = 0, .w = BUTTONS_W * 0.6, .h = BUTTONS_H * 0.58};

}

