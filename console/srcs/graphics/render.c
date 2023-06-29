#include "../includes/splendor.h"

void renderPlayer(Context *ctx, Player *player)
{
	int i;
	SDLX_Display *display;

	display = SDLX_DisplayGet();

	// player->pointSprite.src->x = ctx->assets.textSrc.x + (player->points * ctx->assets.textSrc.w);

	SDLX_RenderQueuePush(&player->pointSprite);


	for (i = 0; i < ESSENCE_TYPES; i++)
	{
		// player->ressources[i].src->x = ctx->assets.textSrc.x + (player->tokens[i] * ctx->assets.textSrc.w);
		SDLX_RenderQueuePush(&player->ressources[i]);
	}

	for (i = 0; i < player->potionCount; i++)
	{
		SDLX_RenderQueuePush(&player->owned[i].sprite);
	}
}

void renderBoard(Context *ctx)
{
	int i;

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);

	for (i = 0; i < ROW_COUNT; i++)
	{
		for (int n = 0; n < ctx->board.rows[i].recipeCount; n++)
			SDLX_RenderQueuePush(&ctx->board.rows[i].recipes[n].sprite);
	}

	for (i = 0; i < MAX_MASTER_POTIONS; i++)
	{
		SDLX_RenderQueuePush(&ctx->board.titles[i].sprite);
	}
	SDLX_RenderResetColour(ctx->display);
}

void render_connect_screen(Context *ctx)
{

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ctx->players[i].status == DISCONNECTED)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
		else if (ctx->players[i].status == READY)
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 255, 0x0,255);
		else if (ctx->players[i].status == CONNECTED)
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 255, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->connectscreen.playerSprites[i].dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->connectscreen.playerStatus[i].dst);
	}
	for (int i = 0; i < ctx->playerCount; i++)
	{
		SDLX_RenderQueuePush(&ctx->connectscreen.playerSprites[i]);
		SDLX_RenderQueuePush(&ctx->connectscreen.playerStatus[i]);
	}
	// SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
	// SDL_RenderDrawRect(ctx->display->renderer, &ctx->connectscreen.status);
	SDLX_RenderResetColour(ctx->display);
}
