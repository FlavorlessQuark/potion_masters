#include "../includes/splendor.h"
#include <time.h>

# define TOP_ROW_COUNT 10
# define MID_ROW_COUNT 10
# define BOT_ROW_COUNT 10

# define ROW_CARD_COUNT 4

# define UI_PLAYER_LEFT 0
# define UI_BOARD 1
# define UI_PLAYER_RIGHT 2

# define TOKEN_ROW 0
# define TOP_CARD_ROW 1
# define MID_CARD_ROW 2
# define BOT_CARD_ROW 3
# define TITLE_ROW 4

void initPlayers(Context *ctx)
{
	// card->alignment = SDLX
	char *handles;
	int offset;


	// handles = SDL_calloc(MAX_PLAYERS, HANDLE_LEN);
	// offset = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		memset(ctx->players[i].owned, 0, 5 * sizeof(uint8_t));
		memset(ctx->players[i].tokens, 0, 5 *sizeof(uint8_t));
		ctx->players[i].id = -1;
		ctx->players[i].reserveCount = 0;
		ctx->players[i].handle = NULL;
		// ctx->players[i].handle = handles + offset;
		// offset += HANDLE_LEN;
	}

}

void initRowCards(Row *row, SDLX_RectContainer *container)
{
	//Parse file data here

	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon._dst = container->elems[0]._boundingBox;
	for (int i = 0; i < row->remainCount; i++)
	{
		row->remaining[i] = (Card) {.cost[0] = 1, .cost[1] = 1, .cost[2] = 1, .cost[3] = 1, .points = rand() % 4, .type = rand() % 4};
		SDLX_SpriteCreate(&row->remaining[i].sprite, 1, NULL);
	}

	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		row->cardButton[i]._boundingBox = container->elems[i + 1]._boundingBox;
	}

	replaceCard(row, 0);
	replaceCard(row, 1);
	replaceCard(row, 2);
	replaceCard(row, 3);

}

void initRows(Context *ctx, SDLX_RectContainer *root)
{
	ctx->board.rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	ctx->board.rows[MID_ROW].remainCount = MID_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remainCount = BOT_ROW_COUNT;


	ctx->cards = SDL_calloc(
		TOP_ROW_COUNT + MID_ROW_COUNT + BOT_ROW_COUNT,
		sizeof(Card));
	ctx->board.rows[TOP_ROW].remaining = ctx->cards + 0;
	ctx->board.rows[MID_ROW].remaining = ctx->cards + TOP_ROW_COUNT;
	ctx->board.rows[BOT_ROW].remaining = ctx->cards + MID_ROW_COUNT;

	// Init textures here
	initRowCards(&ctx->board.rows[TOP_ROW], &root->containers[TOP_CARD_ROW]);
	initRowCards(&ctx->board.rows[MID_ROW], &root->containers[MID_CARD_ROW]);
	initRowCards(&ctx->board.rows[BOT_ROW], &root->containers[BOT_CARD_ROW]);

}

SDLX_RectContainer *initUI(void)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig("assets/UIconfig");
	SDLX_ContainerUpdate(root, NULL);

	return root;
}


Context *init()
{
	Context *ctx;
	SDLX_RectContainer *root;

	srand(time(NULL));
	SDLX_InitDefault();
	ctx = SDL_calloc(1, sizeof(Context));
	ctx->display = SDLX_DisplayGet();
	ctx->board.remainingTitles = MAX_TITLES;
	ctx->state = CONNECT_SCREEN;
	ctx->playerCount = 0;
	root = initUI();
	initRows(ctx, &root->containers[UI_BOARD]);
	initPlayers(ctx);
	cleanupUIConfig(root);
	SDL_free(root);

	return ctx;
}
