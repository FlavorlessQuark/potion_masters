#include "../includes/splendor.h"
#include <time.h>
#include "../includes/table.h"

#define TOP_ROW 0
#define MID_ROW 1
#define BOT_ROW 2

#define TOP_ROW_COUNT 10
#define MID_ROW_COUNT 10
#define BOT_ROW_COUNT 0

#define ROW_CARD_COUNT 4

void init_new_game(Context *ctx);
void fill_board(Context *ctx);
void fill_player(Context *ctx, uint8_t id);

Context *init()
{
	Context *ctx;
	SDL_Surface *surf;

	srand(time(NULL));

	ctx = SDL_calloc(1, sizeof(Context));
	init_UI(ctx);
	init_connectScreen(ctx);
	init_new_game(ctx);
	ctx->state = CONNECT_SCREEN;
	// ctx->state = PLAYING;
	ctx->playerCount = 0;

	return ctx;
}

void init_new_game(Context *ctx)
{
	SDLX_RenderResetColour(ctx->display);
	SDL_SetRenderTarget(ctx->display->renderer, ctx->display->background);
	SDL_SetTextureBlendMode(ctx->display->background, SDL_BLENDMODE_BLEND);

	fill_board(ctx);

	for (int i = 0; i < ctx->playerCount; ++i)
		fill_player(ctx, i);

	SDL_Log("Init new game");
	SDL_SetRenderTarget(ctx->display->renderer, NULL);
}

void initRowPotions(Context *ctx, int level)
{
	Row *row;

	row = &ctx->board.rows[level];
	row->recipeCount = MAX_ROWCARD;
	SDL_Log("Gnereating row %d", level);
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		generatePotion(ctx, &row->recipes[i], level);

	}
}

void fill_board(Context *ctx)
{
	SDL_Rect src;

	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;

	ctx->board.rows[TOP_ROW].recipeCount = ROW_CARD_COUNT;
	ctx->board.rows[MID_ROW].recipeCount = ROW_CARD_COUNT;
	ctx->board.rows[BOT_ROW].recipeCount = ROW_CARD_COUNT;

	SDL_Log("fil board");
	for (int i = 0; i < ROW_COUNT; i++)
		initRowPotions(ctx, i);
}

void fill_player(Context *ctx, uint8_t id)
{
	int i;

	SDL_Rect src;
	char name[9] = {'P', 'L', 'A', 'Y', 'E', 'R', ' ', (id + 1) + '0', '\0'};

	memset(ctx->players[id].tokens, 3, 5 * sizeof(uint8_t));
	ctx->players[id].potionCount = 0;
	ctx->players[id].actionsRemaining = 0;
	for (int i = 0; i < MAX_POTIONS; ++i)
	{
		ctx->players[id].owned[i].fill = 0;
	}

	draw_player_essences(ctx, &ctx->players[id]);
}

#define START 3000
void init_connectScreen(Context *ctx)
{
	ctx->connectscreen.counter = START;
}
