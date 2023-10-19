#include "../includes/splendor.h"

void init_connect_screen(Context *ctx)
{
	SDLX_SpriteCreate(&ctx->connection.bg, 1, ctx->assets.texUI);
	SDLX_SpriteCreate(&ctx->connection.connectSprite, 1, SDL_CreateTexture(
			ctx->display->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			310, 180
			));
	SDLX_SpriteCreate(&ctx->connection.name, 1, SDL_CreateTexture(
			ctx->display->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			500, 500
			));
	SDL_SetTextureBlendMode(ctx->connection.name.texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(ctx->connection.connectSprite.texture, SDL_BLENDMODE_NONE);
	SDLX_ButtonCreate(&ctx->connection.connectButton, &ctx->connection.connectSprite._dst);
}

