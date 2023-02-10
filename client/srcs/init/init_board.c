#include "../includes/splendor.h"

# define TOKEN_ROW 0
# define TOP_CARD_ROW 1
# define MID_CARD_ROW 2
# define BOT_CARD_ROW 3
# define ROW_CARD_COUNT 4

void initRowCards(Row *row, SDLX_RectContainer *root)
{
	//Parse texture data here

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

	// initTokens(&ctx->board.rows[TOKEN_ROW], &root->containers[TOKEN_ROW]);
	initRowCards(&ctx->board.rows[TOP_ROW], &root->containers[0]);
	initRowCards(&ctx->board.rows[MID_ROW], &root->containers[1]);
	initRowCards(&ctx->board.rows[BOT_ROW], &root->containers[2]);
	SDLX_ButtonCreate(&ctx->board.switchMode, NULL);
	ctx->board.switchMode._boundingBox.x = ctx->display->win_w - 55;
	ctx->board.switchMode._boundingBox.y =( ctx->display->win_h / 2) - 25;
	ctx->board.switchMode._boundingBox.w = 50;
	ctx->board.switchMode._boundingBox.h = 50;

}
