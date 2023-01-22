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

	SDL_Rect rect;

	rect.x = 100;
	rect.y = 300;
	rect.w = 50;
	rect.h = 50;
	memset(ctx->players[0].owned, 0, 5 * sizeof(uint8_t));
	ctx->players[0].reserveCount = 0;
	memset(ctx->players[0].tokens, 0, 5 *sizeof(uint8_t));
	SDLX_ButtonCreate(&ctx->players[0].reservedButton[0], NULL);
	SDLX_ButtonCreate(&ctx->players[0].reservedButton[1], NULL);
	SDLX_ButtonCreate(&ctx->players[0].reservedButton[2], NULL);
	SDLX_ButtonCreate(&ctx->players[0].reservedButton[3], NULL);
	ctx->players[0].reservedButton[0]._boundingBox = rect;
	rect.x += 60;
	ctx->players[0].reservedButton[1]._boundingBox = rect;
	rect.x += 60;
	ctx->players[0].reservedButton[2]._boundingBox = rect;
	rect.x += 60;
	ctx->players[0].reservedButton[3]._boundingBox = rect;
}

void initBoardItems(Context *ctx)
{
	SDLX_RectContainer container;

	SDLX_ContainerElemCreate(&container.self, NULL, 0, 0, 0);
	container.containerCount = 3;
	container.containers = SDL_calloc(container.containerCount, sizeof(SDLX_RectContainer));
	container.elemCount = 0;
	container.alignDirection = SDLX_ALIGN_HORIZONTAL;
	container.alignment = SDLX_SPACE_EVEN;
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

void initRows(Row *rows, SDLX_RectContainer *root)
{
	Card *allocated;


	rows[TOP_ROW].remainCount = TOP_ROW_COUNT;
	rows[MID_ROW].remainCount = MID_ROW_COUNT;
	rows[BOT_ROW].remainCount = BOT_ROW_COUNT;


	allocated = SDL_calloc(
		TOP_ROW_COUNT + MID_ROW_COUNT + BOT_ROW_COUNT,
		sizeof(Card));
	rows[TOP_ROW].remaining = allocated + 0;
	rows[MID_ROW].remaining = allocated + TOP_ROW_COUNT;
	rows[BOT_ROW].remaining = allocated + MID_ROW_COUNT;

	// Init textures here
	initRowCards(&rows[TOP_ROW], &root->containers[TOP_CARD_ROW]);
	initRowCards(&rows[MID_ROW], &root->containers[MID_CARD_ROW]);
	initRowCards(&rows[BOT_ROW], &root->containers[BOT_CARD_ROW]);

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

	root = initUI();
	initRows(ctx->board.rows, &root->containers[UI_BOARD]);
	initPlayers(ctx);


	return ctx;
}
