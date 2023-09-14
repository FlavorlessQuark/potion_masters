#include "../includes/splendor.h"

# define MAX_TAKE 3

typedef struct board_tokens {
	uint8_t  taken[ESSENCE_TYPES];
	uint8_t  count;
	uint8_t  max;
	int  lock;

}			board_tokens;

static board_tokens tokens = {.lock = -1, .max = MAX_TAKE, .count = 0};

void set_cards_active(Context *ctx, int enabled)
{
	for (int x = 0; x < ROW_COUNT; x++)
	{
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			ctx->board.rows[x].cardButton[i].enabled = enabled;
		}
	}
}

void board_screen(Context *ctx)
{
	// int total;

	// if (ctx->switchMode.triggered == SDLX_KEYDOWN)
	// 	ctx->state = 0;

	if (ctx->board.overlay.selected != NULL)
	{
		if (ctx->board.overlay.exit.button.triggered == SDLX_KEYDOWN)
		{
			set_cards_active(ctx, SDL_TRUE);
			ctx->board.overlay.selected = NULL;
			ctx->board.overlay.exit.button.triggered = 0;
		}
	}
	else {
		for (int x = 0; x < ROW_COUNT; x++)
		{
			for (int i = 0; i < MAX_ROWCARD; i++)
			{
				if (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYDOWN)
				{
					ctx->board.overlay.selected = &ctx->board.rows[x].card[i];
					ctx->board.rows[x].cardButton[i].triggered = 0;
					set_cards_active(ctx, SDL_FALSE);
					break ;
				}
			}
		}
	}


	render_board_screen(ctx);
}


void render_board_screen(Context *ctx)
{
	// SDL_RenderCopy(ctx->display->renderer, ctx->board.bg, NULL, NULL);
	// SDLX_RenderQueuePush(&ctx->switchSprite);

	for (int x = 0; x < ROW_COUNT; x++)
	{
		// SDLX_RenderQueuePush(&ctx->board.rows[x].rowIcon);
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
		// 	if (ctx->board.rows[x].revealed[i].id > 0)
		// 	{
		// 		SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].sprite);
		// 	}
			if (ctx->board.rows[x].cardButton[i].state == SDLX_KEYHELD)
				SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
			else
				SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].card[i].sprite.dst);

		}
		// SDL_Log("HELLO :)");
		// SDLX_SpritePrint(&ctx->board.rows[x].card->sprite);
	}
	if (ctx->board.overlay.selected != NULL)
	{
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.overlay.background.dst);
		if (ctx->board.overlay.exit.button.state == SDLX_KEYHELD)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 255, 0, 255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.overlay.exit.sprite.dst);

	}


	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
