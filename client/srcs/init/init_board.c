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

void initRowCards(Row *row, SDLX_RectContainer *root, SDL_Texture *tex)
{
	//Parse texture data here
	SDLX_SpriteCreate(&row->rowIcon, 1, NULL);
	row->rowIcon._dst = root->elems[0]._boundingBox;
	for (int i = 0; i < ROW_CARD_COUNT; i++)
	{
		SDLX_ButtonCreate(&row->cardButton[i], NULL);
		SDLX_SpriteCreate(&row->revealed[i].sprite, 1, tex);
		row->cardButton[i]._boundingBox = root->elems[i + 1]._boundingBox;
		row->revealed[i].sprite._dst = root->elems[i + 1]._boundingBox;
		row->revealed[i].sprite._src.h = CARD_H;
		row->revealed[i].sprite._src.w = CARD_W;
	}

}

void init_board_screen(Context *ctx)
{
	SDLX_RectContainer *root;
	SDL_Surface *surf;
	SDL_Texture *tex;

	root = parseUI("assets/UI/boardUI");
	surf = IMG_Load("assets/cardData/Cards.png");
	tex = SDL_CreateTextureFromSurface(ctx->display->renderer, surf);
	SDL_Log("SURF %p text %p",surf, tex);

	SDL_FreeSurface(surf);

	initTokens(&ctx->board, &root->containers[TOKENS]);
	initRowCards(&ctx->board.rows[TOP_ROW], &root->containers[CARDS].containers[TOP_ROW], tex);
	initRowCards(&ctx->board.rows[MID_ROW], &root->containers[CARDS].containers[MID_ROW], tex);
	initRowCards(&ctx->board.rows[BOT_ROW], &root->containers[CARDS].containers[BOT_ROW], tex);
	SDLX_ButtonCreate(&ctx->board.switchMode, NULL);
	ctx->board.switchMode._boundingBox.x = root->containers[SWITCH].elems[0]._boundingBox.x;
	ctx->board.switchMode._boundingBox.y = root->containers[SWITCH].elems[0]._boundingBox.y;
	ctx->board.switchMode._boundingBox.w = root->containers[SWITCH].elems[0]._boundingBox.w;
	ctx->board.switchMode._boundingBox.h = root->containers[SWITCH].elems[0]._boundingBox.h;

}
