#include "../includes/splendor.h"

# define MAX_TAKE 3

static uint8_t  taken[TOK_COUNT];
static uint8_t  takenCount;
void board_screen(Context *ctx)
{
	static int  lock = -1;
	static uint8_t max = MAX_TAKE;
	int total;

	if (ctx->board.switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 0;
	total = 0;
	for (int i = 0; i < CARD_TYPES; i++)
		total += ctx->player.tokens[i];

	total += takenCount;
	SDL_Log("Total %d", total);
	for (int i = 0; i < TOK_COUNT && total < 10; i++)
	{
		// SDL_Log("?? %d%d %d | %d %d -> %d", lock < 0, lock == i, takenCount < MAX_TAKE,  ctx->board.tokenButton[i].triggered == SDLX_KEYDOWN, (lock < 0 || lock == i) && (takenCount < MAX_TAKE && ctx->board.tokenButton[i].triggered == SDLX_KEYDOWN));
		if  (((lock < 0 ) || lock == i) && (takenCount < MAX_TAKE && ctx->board.tokenButton[i].triggered == SDLX_KEYDOWN))
		{
			if (++taken[i] > 1)
			{
				lock = i;
				max = MAX_TAKE - 1;
			}
			SDL_Log("Taking %d -> %d", i, takenCount);
			takenCount++;
			total++;
			SDL_Log("TOOOTAAAL %d", total);
		}
	}

	if (ctx->board.tokenButton[TOKEN_BUTTON_RESET].triggered == SDLX_KEYDOWN)
	{
		SDL_memset(taken, 0, sizeof(uint8_t) * CARD_TYPES);
		takenCount = 0;
		lock = -1;
		max = MAX_TAKE;
	}
	else if (ctx->board.tokenButton[TOKEN_BUTTON_CONFIRM].triggered == SDLX_KEYDOWN)
	{
		for (int i = 0; i < TOK_COUNT; i++)
			ctx->player.tokens[i] += taken[i];
		sendTakeTokens(ctx, taken);
		SDL_memset(taken, 0, sizeof(uint8_t) * CARD_TYPES);
		ctx->state = 0;
		takenCount = 0;
		lock = -1;
		max = MAX_TAKE;
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
				ctx->buyscreen.showSelected.src = ctx->buyscreen.selected->sprite.src;
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
