#include "../includes/splendor.h"

# define TOKEN_ROW 0
# define TOP_CARD_ROW 1
# define MID_CARD_ROW 2
# define BOT_CARD_ROW 3
# define ROW_CARD_COUNT 4

# define CARDS 0
# define TOKENS 1
# define SWITCH 2

void initTokens(Board *board, SDLX_RectContainer *root)
{
	for (int i = 0; i < TOK_COUNT + 2; i++)
	{
		SDLX_SpriteCreate(&board->tokenSprites[i], 1, NULL);
		SDLX_ButtonCreate(&board->tokenButton[i], board->tokenSprites[i].dst);
		board->tokenSprites[i]._dst = root->elems[i]._boundingBox;
	}
}

void initRowCards(Row *row, SDLX_RectContainer *root)
{
	//Parse texture data here
	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon._dst = root->elems[0]._boundingBox;
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		row->cardButton[i]._boundingBox = root->elems[i + 1]._boundingBox;
	}

}

void init_board_screen(Context *ctx)
{
	SDLX_RectContainer *root;

	root = parseUI("assets/UI/boardUI");

	initTokens(&ctx->board, &root->containers[TOKENS]);
	initRowCards(&ctx->board.rows[TOP_ROW], &root->containers[CARDS].containers[TOP_ROW]);
	initRowCards(&ctx->board.rows[MID_ROW], &root->containers[CARDS].containers[MID_ROW]);
	initRowCards(&ctx->board.rows[BOT_ROW], &root->containers[CARDS].containers[BOT_ROW]);
	SDLX_ButtonCreate(&ctx->board.switchMode, NULL);
	ctx->board.switchMode._boundingBox.x = root->containers[SWITCH].elems[0]._boundingBox.x;
	ctx->board.switchMode._boundingBox.y = root->containers[SWITCH].elems[0]._boundingBox.y;
	ctx->board.switchMode._boundingBox.w = root->containers[SWITCH].elems[0]._boundingBox.w;
	ctx->board.switchMode._boundingBox.h = root->containers[SWITCH].elems[0]._boundingBox.h;

}
