#include "../includes/splendor.h"

void renderPlayer(Context *ctx, Player *player)
{
	int i;
	SDLX_Display *display;

	display = SDLX_DisplayGet();
	SDL_SetRenderDrawColor(display->renderer, 255, 0,0,255);

	player->pointSprite.src->x = ctx->numbers.x + (player->points * ctx->numbers.w);

	SDLX_RenderQueuePush(&player->pointSprite);

	for (i = 0; i < CARD_TYPES; i++)
	{
		player->ressources[i].src->x = ctx->numbers.x + (player->tokens[i] * ctx->numbers.w);
		player->permanents[i].src->x = ctx->numbers.x + (player->owned[i] * ctx->numbers.w);
		SDLX_RenderQueuePush(&player->ressources[i]);
		SDLX_RenderQueuePush(&player->permanents[i]);
	}
	// player->ressources[i].src->x = ctx->numbers.x + (player->tokens[i] * ctx->numbers.w);
	// SDLX_RenderQueuePush(&player->ressources[i]);

	for (i = 0; i < player->reserveCount; i++)
	{
		SDLX_RenderQueuePush(&player->reserved[i].sprite);
	}

	SDL_SetRenderDrawColor(display->renderer, 0, 0,0,255);

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
		{
			SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[n].sprite);
			ctx->board.rows[i].revealed[n].costSprite[0]._src.x = ctx->numbers.x + (ctx->board.rows[i].revealed[n].cost[0] * ctx->numbers.w);
			ctx->board.rows[i].revealed[n].costSprite[1]._src.x = ctx->numbers.x + (ctx->board.rows[i].revealed[n].cost[1] * ctx->numbers.w);
			ctx->board.rows[i].revealed[n].costSprite[2]._src.x = ctx->numbers.x + (ctx->board.rows[i].revealed[n].cost[2] * ctx->numbers.w);
			ctx->board.rows[i].revealed[n].costSprite[3]._src.x = ctx->numbers.x + (ctx->board.rows[i].revealed[n].cost[3] * ctx->numbers.w);
			SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[n].costSprite[0]);
			SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[n].costSprite[1]);
			SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[n].costSprite[2]);
			SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[n].costSprite[3]);
		}
		// SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[1].sprite);
		// SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[2].sprite);
		// SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[3].sprite);
	}
	for (i = 0; i < TOK_COUNT; i++)
	{
		ctx->board.tokenUI[i].src->x = ctx->numbers.x + (ctx->board.tokens[i] * ctx->numbers.w);
		SDLX_RenderQueuePush(&ctx->board.tokenUI[i]);
	}
	if (count == 100)
		count = 0;
	count++;
	for (i = 0; i < MAX_TITLES; i++)
	{
		SDL_RenderDrawRect(ctx->display->renderer,  ctx->board.titleUI[i].dst);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0,0,255);
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
	SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 0x0, 255);
}
