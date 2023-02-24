#include "../includes/splendor.h"

void renderPlayer(PlayerUI *player)
{
	int i;
	SDLX_Display *display;

	display = SDLX_DisplayGet();
	SDL_SetRenderDrawColor(display->renderer, 255, 0,0,255);

	SDL_RenderDrawRect(display->renderer, &player[0].nameTag);
	SDL_RenderDrawRect(display->renderer, &player[0].pointsTag);

	for (i = 0; i < CARD_TYPES; i++)
	{
		SDL_RenderDrawRect(display->renderer, player[0].ressourceIcon[i].dst);
		SDL_RenderDrawRect(display->renderer, player[0].permanentIcon[i].dst);
	}
	SDL_RenderDrawRect(display->renderer, player[0].ressourceIcon[i].dst);

	for (i = 0; i < MAX_RESERVE; i++)
	{
		SDL_RenderDrawRect(display->renderer, player[0].reservedIcon[i].dst);
	}

	SDL_SetRenderDrawColor(display->renderer, 0, 0,0,255);

}

void renderBoard(Context *ctx)
{
	int i;

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);

	for (i = 0; i < ROW_COUNT; i++)
	{
		// SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[i].rowIcon.dst);
		SDLX_RenderQueuePush(&ctx->board.rows[i].rowIcon);
		SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[0].sprite);
		SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[1].sprite);
		SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[2].sprite);
		SDLX_RenderQueuePush(&ctx->board.rows[i].revealed[3].sprite);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[i].revealed[0].sprite.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[i].revealed[1].sprite.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[i].revealed[2].sprite.dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[i].revealed[3].sprite.dst);
	}
	for (i = 0; i < TOK_COUNT; i++)
	{
		SDLX_RenderQueuePush(&ctx->board.tokenUI[i]);
		// SDLX_SpritePrint(&ctx->board.tokenUI[i]);

		SDL_RenderDrawRect(ctx->display->renderer,  ctx->board.tokenUI[i].dst);
	}
	for (i = 0; i < MAX_TITLES; i++)
	{
		SDL_RenderDrawRect(ctx->display->renderer,  ctx->board.titleUI[i].dst);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0,0,255);
}

void render_connect_screen(Context *ctx)
{
	int space;
	SDL_Rect rect;

	rect.h = ctx->display->win_h / 4;
	rect.w = ctx->display->win_w / 7;
	rect.y = ctx->display->win_h / 4;
	rect.x = ctx->display->win_w / 10;
	space = rect.x;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		// SDL_Log("Player %d -> %d | %s", i, ctx->players[i].status, ctx->players[i].handle);
		if (ctx->players[i].status == DISCONNECTED)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
		else if (ctx->players[i].status == READY)
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 255, 0x0,255);
		else if (ctx->players[i].status == CONNECTED)
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 255, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->connectscreen.playerSprites[i].dst);
		rect.x += space + rect.w;
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
	SDL_RenderDrawRect(ctx->display->renderer, &ctx->connectscreen.status);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 0x0, 255);
}
