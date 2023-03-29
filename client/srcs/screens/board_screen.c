#include "../includes/splendor.h"

# define MAX_TAKE 3

typedef struct board_tokens {
	uint8_t  taken[TOK_COUNT];
	uint8_t  count;
	uint8_t  max;
	int  lock;

}			board_tokens;

static board_tokens tokens = {.lock = -1, .max = MAX_TAKE, .count = 0};

void reset_tokens(void)
{
	SDL_memset(tokens.taken, 0, sizeof(uint8_t) * CARD_TYPES);
	tokens.count = 0;
	tokens.lock = -1;
	tokens.max = MAX_TAKE;
}

void try_take_token(int index)
{
	if (tokens.count >= tokens.max)
		return ;
	if (tokens.lock == -1)
	{
		if (tokens.count == 2 && tokens.taken[index] > 0)
			return ;
		if (tokens.taken[index] >= 1)
		{
			tokens.lock = index;
			tokens.max -= 1;
		}
	}
	else if (tokens.lock != index)
		return;
	tokens.count++;
	tokens.taken[index]++;
}

void board_screen(Context *ctx)
{
	int total;

	if (ctx->switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 0;
	total = 0;
	for (int i = 0; i < CARD_TYPES; i++)
		total += ctx->player.tokens[i];

	total += tokens.count;
	for (int i = 0; i < TOK_COUNT && total < 10; i++)
	{
		if ( ctx->board.tokenButton[i].triggered == SDLX_KEYDOWN)
		{
			SDL_Log("? :)");
			try_take_token(i);
			break ;
		}
	}

	if (ctx->board.tokenButton[TOKEN_BUTTON_RESET].triggered == SDLX_KEYDOWN)
		reset_tokens();
	else if (ctx->board.tokenButton[TOKEN_BUTTON_CONFIRM].triggered == SDLX_KEYDOWN)
	{
		for (int i = 0; i < TOK_COUNT; i++)
			ctx->player.tokens[i] += tokens.taken[i];
		sendTakeTokens(ctx, tokens.taken);
		reset_tokens();
	}

	for (int x = 0; x < ROW_COUNT; x++)
	{
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			if (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYDOWN)
			{
				ctx->state = 2;
				ctx->buyscreen.cardOrigin = 0;
				ctx->buyscreen.selected = &ctx->board.rows[x].revealed[i];
				// ctx->buyscreen.showSelected.src = ctx->buyscreen.selected->sprite.src;
				ctx->buyscreen.showSelected.texture = ctx->buyscreen.selected->sprite.texture;
			}
		}
	}

	render_board_screen(ctx);
}


void render_board_screen(Context *ctx)
{
	SDL_RenderCopy(ctx->display->renderer, ctx->board.bg, NULL, NULL);
	SDLX_RenderQueuePush(&ctx->switchSprite);
	for (int x = 0; x < ROW_COUNT; x++)
	{
		SDLX_RenderQueuePush(&ctx->board.rows[x].rowIcon);
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			if (ctx->board.rows[x].revealed[i].id > 0)
			{
				SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].sprite);
			}
		}
	}

	for (int i = 0; i < CARD_TYPES; i++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer,
						255 * (ctx->board.tokenButton[i].triggered == SDLX_KEYHELD),
						255 * (ctx->board.tokenButton[i].state == SDLX_FOCUS_STAY),
						255,
						255);
		ctx->board.tokenCount[i]._src.x = ctx->nums.x + (ctx->nums.w * (ctx->board.tokens[i] - tokens.taken[i]));
		ctx->board.tokenTaken[i]._src.x = ctx->nums.x + (ctx->nums.w * tokens.taken[i]);
		SDLX_RenderQueuePush(&ctx->board.tokenTaken[i]);
		SDLX_RenderQueuePush(&ctx->board.tokenCount[i]);
	}
	ctx->board.tokenCount[CARD_TYPES]._src.x = ctx->nums.x + (ctx->nums.w * ctx->board.tokens[CARD_TYPES]);
	SDLX_RenderQueuePush(&ctx->board.tokenCount[CARD_TYPES]);
	// SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	// SDL_RenderDrawRect(ctx->display->renderer, ctx->board.switchMode.boundingBox);
	// SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
