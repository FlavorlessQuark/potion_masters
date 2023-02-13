#include "../includes/splendor.h"

# define MAX_TAKE 3

void board_screen(Context *ctx)
{
	static uint8_t  taken[TOK_COUNT];
	static uint8_t  takenCount;
	static uint8_t  lock = -1;
	static uint8_t max = MAX_TAKE;

	if (ctx->board.switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 0;

	for (int i = 0; i < TOK_COUNT; i++)
	{
		if  ((lock < 0 || lock == i) && takenCount < MAX_TAKE && ctx->board.tokenButton[i].triggered == SDLX_KEYDOWN)
		{
			if (++taken[i] > 1)
			{
				lock = i;
				max = MAX_TAKE - 1;
			}
			takenCount++;
		}
	}

	if (ctx->board.tokenButton[TOKEN_BUTTON_RESET].triggered == SDLX_KEYDOWN)
	{
		SDL_memset(taken, 0, sizeof(uint8_t));
		takenCount = 0;
		lock = -1;
		max = MAX_TAKE;
	}
	else if (ctx->board.tokenButton[TOKEN_BUTTON_CONFIRM].triggered == SDLX_KEYDOWN)
	{
		for (int i = 0; i < TOK_COUNT; i++)
			ctx->player.tokens[i] += taken[i];
		sendTakeTokens(ctx, taken);
		SDL_memset(taken, 0, sizeof(uint8_t));
		takenCount = 0;
		lock = -1;
		max = MAX_TAKE;
	}

	render_board_screen(ctx);
}


void render_board_screen(Context *ctx)
{
	for (int x = 0; x < ROW_COUNT; x++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].rowIcon.dst);
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			if (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYDOWN)
			{
				ctx->state = 2;
				ctx->buyscreen->cardOrigin = 0;
			}
			SDL_SetRenderDrawColor(ctx->display->renderer,
							255 * (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYHELD),
							255 * (ctx->board.rows[x].cardButton[i].state == SDLX_FOCUS_STAY),
							255,
							255);
			SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].cardButton[i].boundingBox);
		}
	}

	for (int i = 0; i < TOK_COUNT + 2; i++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer,
						255 * (ctx->board.tokenButton[i].triggered == SDLX_KEYHELD),
						255 * (ctx->board.tokenButton[i].state == SDLX_FOCUS_STAY),
						255,
						255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.tokenButton[i].boundingBox);
	}

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.switchMode.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
