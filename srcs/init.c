#include "../includes/splendor.h"
#include <time.h>

# define TOP_ROW_COUNT 10
# define MID_ROW_COUNT 10
# define BOT_ROW_COUNT 10

# define ROW_CARD_COUNT 4

void initPlayerCard(SDLX_RectContainer *card)
{
	// card->alignment = SDLX
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

void initRowCards(Row *row, int y)
{
	SDL_Rect rect;

	rect.h = 50;
	rect.w = 50;
	rect.x = 50;
	rect.y = y;
	//Parse file data here

	for (int i = 0; i < row->remainCount; i++)
	{
		row->remaining[i] = (Card) {.cost[0] = 1, .cost[1] = 1, .cost[2] = 1, .cost[3] = 1, .points = rand() % 4};
		SDLX_SpriteCreate(&row->remaining[i].sprite, 1, NULL);
	}

	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		row->cardButton[i]._boundingBox = rect;
		rect.x += 70;
	}

	replaceCard(row, 0);
	replaceCard(row, 1);
	replaceCard(row, 2);
	replaceCard(row, 3);

}

void initRows(Row *rows)
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
	initRowCards(&rows[TOP_ROW], 60);
	initRowCards(&rows[MID_ROW], 120);
	initRowCards(&rows[BOT_ROW], 180);

}


Context *init()
{
	Context *ctx;

	srand(time(NULL));
	SDLX_InitDefault();
	ctx = SDL_calloc(1, sizeof(Context));
	ctx->display = SDLX_DisplayGet();
	ctx->board.remainingTitles = MAX_TITLES;
	initRows(ctx->board.rows);


	return ctx;
}
