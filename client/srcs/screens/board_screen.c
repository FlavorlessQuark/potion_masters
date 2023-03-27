#include "../includes/splendor.h"

# define MAX_TAKE 3

typedef struct tokens {
	uint8_t taken[CARD_TYPES];
	uint8_t count;
	uint8_t lock;
	uint8_t max;
}	tokens;



void reset_tokens()
{

}

void try_take_token(Context *ctx, tokens *toks)
{
	if (toks->count >= toks->max)
		return ;
	for (int i = 0; i < CARD_TYPES; i++)
	{
		if (ctx->board.tokenButton[i].triggered == SDLX_KEYDOWN)
		{
			if (toks->lock != i)
				break ;
			if (toks->taken[i] > 0 && toks->count > toks->taken[i])
				break ;
			if (toks->taken[i] > 0 )
				toks->lock = i;
			toks->count++;
			toks->taken[i]++;
			return ;
		}
	}
}

void board_screen(Context *ctx)
{
	static tokens toks = {.max = MAX_TAKE, .lock = -1};
	int total;

	if (ctx->board.switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 0;
	total = 0;
	for (int i = 0; i < CARD_TYPES; i++)
		total += ctx->player.tokens[i];

	total += toks.count;
	SDL_Log("Total %d", total);


	if (ctx->board.tokenButton[TOKEN_BUTTON_RESET].triggered == SDLX_KEYDOWN)
	{
		reset_tokens();
	}
	else if (ctx->board.tokenButton[TOKEN_BUTTON_CONFIRM].triggered == SDLX_KEYDOWN)
	{
		sendTakeTokens(ctx, toks.taken);
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
	for (int x = 0; x < ROW_COUNT; x++)
	{
		SDLX_RenderQueuePush(&ctx->board.rows[x].rowIcon);
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			if (ctx->board.rows[x].revealed[i].id > 0)
			{
				SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].sprite);
				// ctx->board.rows[x].revealed[i].costSprite[0]._src.x = ctx->nums.x + (ctx->board.rows[x].revealed[i].cost[0] * ctx->nums.w);
				// ctx->board.rows[x].revealed[i].costSprite[1]._src.x = ctx->nums.x + (ctx->board.rows[x].revealed[i].cost[1] * ctx->nums.w);
				// ctx->board.rows[x].revealed[i].costSprite[2]._src.x = ctx->nums.x + (ctx->board.rows[x].revealed[i].cost[2] * ctx->nums.w);
				// ctx->board.rows[x].revealed[i].costSprite[3]._src.x = ctx->nums.x + (ctx->board.rows[x].revealed[i].cost[3] * ctx->nums.w);
				// SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].costSprite[0]);
				// SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].costSprite[1]);
				// SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].costSprite[2]);
				// SDLX_RenderQueuePush(&ctx->board.rows[x].revealed[i].costSprite[3]);
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
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.tokenButton[i].boundingBox);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.tokenCount[i].dst);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.tokenTaken[i].dst);
		ctx->board.tokenCount[i]._src.x = ctx->nums.x + (ctx->nums.w * (ctx->board.tokens[i] - taken[i]));
		ctx->board.tokenTaken[i]._src.x = ctx->nums.x + (ctx->nums.w * taken[i]);
		SDLX_RenderQueuePush(&ctx->board.tokenTaken[i]);
		SDLX_RenderQueuePush(&ctx->board.tokenCount[i]);
	}
	ctx->board.tokenCount[CARD_TYPES]._src.x = ctx->nums.x + (ctx->nums.w * ctx->board.tokens[CARD_TYPES]);
	SDLX_RenderQueuePush(&ctx->board.tokenCount[CARD_TYPES]);
	// SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	// SDL_RenderDrawRect(ctx->display->renderer, ctx->board.switchMode.boundingBox);
	// SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
