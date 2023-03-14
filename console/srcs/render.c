#include "../includes/splendor.h"

void renderPlayer(Context *ctx, Player *player)
{
	int i;
	SDLX_Display *display;

	display = SDLX_DisplayGet();

	player->pointSprite.src->x = ctx->numbers.x + (player->points * ctx->numbers.w);

	SDLX_RenderQueuePush(&player->pointSprite);

	for (i = 0; i < CARD_TYPES; i++)
	{
		player->ressources[i].src->x = ctx->numbers.x + (player->tokens[i] * ctx->numbers.w);
		player->permanents[i].src->x = ctx->numbers.x + (player->owned[i] * ctx->numbers.w);
		SDLX_RenderQueuePush(&player->ressources[i]);
		SDLX_RenderQueuePush(&player->permanents[i]);
	}

	for (i = 0; i < player->reserveCount; i++)
		SDLX_RenderQueuePush(&player->reserved[i].sprite);
}

void renderBoard(Context *ctx)
{
	int i;
	static int count;


	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);

	for (i = 0; i < ROW_COUNT; i++)
	{
		SDLX_RenderQueuePush(&ctx->board.rows[i].rowIcon);

		for (int n = 0; n < ctx->board.rows[i].revealedCount; n++)
			SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[n].sprite);
	}
	for (i = 0; i < TOK_COUNT; i++)
	{
		ctx->board.tokenUI[i].src->x = ctx->numbers.x + (ctx->board.tokens[i] * ctx->numbers.w);
		SDLX_RenderQueuePush(&ctx->board.tokenUI[i]);
	}
	if (count == 100)
		count = 0;
	count++;
	// for (i = 0; i < MAX_TITLES; i++)
	// {
	// 	SDL_RenderDrawRect(ctx->display->renderer,  ctx->board.titleUI[i].dst);
	// }
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
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
	SDL_RenderDrawRect(ctx->display->renderer, &ctx->connectscreen.status);
	SDLX_RenderResetColour(ctx->display);
}
