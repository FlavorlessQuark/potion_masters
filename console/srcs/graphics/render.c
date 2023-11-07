#include "../includes/splendor.h"

void renderPlayer(Context *ctx, Player *player)
{
	int i;
	SDLX_Display *display;

	display = SDLX_DisplayGet();

	SDL_RenderCopy(ctx->display->renderer, ctx->assets.mainBg, NULL, NULL);
	SDLX_RenderQueuePush(&player->name);
	for (i = 0; i < ESSENCE_TYPES; i++)
	{
		SDLX_RenderQueuePush(&player->essences[i]);
	}

	for (i = 0; i < player->potionCount; i++)
	{
		SDLX_RenderQueuePush(&player->owned[i].sprite);
	}
}

void renderBoard(Context *ctx)
{
	int i;

	// SDLX_RenderQueuePush(&ctx->assets);
	SDL_RenderCopy( ctx->display->renderer ,ctx->assets.mainBg, NULL, NULL);
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);

	for (i = 0; i < ROW_COUNT; i++)
	{
		for (int n = 0; n < ctx->board.rows[i].recipeCount; n++)
			SDLX_RenderQueuePush(&ctx->board.rows[i].recipes[n].sprite);
	}

	for (i = 0; i < MAX_MASTER_POTIONS; i++)
	{
		SDLX_RenderQueuePush(&ctx->board.master[i].sprite);
	}
	SDLX_RenderResetColour(ctx->display);
}

void render_connect_screen(Context *ctx)
{
	SDLX_RenderQueuePush(&ctx->connectscreen.bg);
	SDLX_RenderQueuePush(&ctx->connectscreen.timer);
	for (int i = 0; i <  ctx->playerCount; i++)
	{
		SDLX_RenderQueuePush(&ctx->connectscreen.playerName[i]);
		SDLX_RenderQueuePush(&ctx->connectscreen.playerStatus[i]);
	}
	SDLX_RenderResetColour(ctx->display);
}
