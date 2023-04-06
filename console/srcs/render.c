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
	{
		SDLX_RenderQueuePush(&player->reserved[i].sprite);
	}
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
		if (ctx->board.tokens[i] <= 9)
			ctx->board.tokenUI[i].src->x = ctx->numbers.x + (ctx->board.tokens[i] * ctx->numbers.w);
		else
		{
			ctx->board.tokenUI[i].src->x = ctx->numbers.x + (9 *  ctx->numbers.w);
			ctx->board.tokenUI[i].src->x += ((ctx->board.tokens[i] - 9) *  ctx->numbers.w * 2);
			ctx->board.tokenUI[i].src->x = ctx->numbers.w * 2;
		}
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

# define FRAME_COUNT 6
# define OFFSET 40
# define HEIGHT 640
# define WIDTH 600
void render_connect_screen(Context *ctx, int *timer)
{
	static uint8_t frame[MAX_PLAYERS] = {0,0,0,0};
	SDL_Rect dst = {0,0, 0, 0};
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ctx->players[i].status == DISCONNECTED)
		{
			if (frame[i] > 0)
				frame[i]--;
			ctx->connectscreen.playerSprites[i].src->x = OFFSET + ((WIDTH + OFFSET) * frame[i]);
		}
		else if (ctx->players[i].status == READY)
		{
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 255, 0x0,255);
		}
		else if (ctx->players[i].status == CONNECTED)
		{
			if (frame[i] < FRAME_COUNT - 1)
			{
				frame[i] += 1;
			}
			ctx->connectscreen.playerSprites[i].src->x = OFFSET + ((WIDTH + OFFSET) * frame[i]);
		}
		SDL_RenderDrawRect(ctx->display->renderer, ctx->connectscreen.playerSprites[i].dst);
		SDLX_RenderQueuePush(&ctx->connectscreen.playerSprites[i]);
	}
	if (timer != NULL)
	{
		char timerStr[6];
		SDL_Rect rect = {0,0, 0, 0};

		TTF_SizeText(ctx->display->defaultFont, "Game starting in:_", &rect.w, &rect.h);
		rect.y = ctx->display->win_h / 2;
		rect.x = ctx->display->win_w / 2 - rect.w / 2;
		SDLX_RenderMessage(ctx->display, &rect, (SDL_Color){255,255,255,255}, "Game starting in: ");
		rect.x += rect.w;
		SDL_itoa(*timer /(FPS * UPDATE_LEN_MS) ,timerStr, 10 );
		TTF_SizeText(ctx->display->defaultFont, timerStr, &rect.w, &rect.h);
		SDLX_RenderMessage(ctx->display, &rect, (SDL_Color){255,255,255,255}, timerStr);
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
	SDL_RenderDrawRect(ctx->display->renderer, &ctx->connectscreen.status);
	SDLX_RenderResetColour(ctx->display);
}
