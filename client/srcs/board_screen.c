#include "../includes/splendor.h"

# define TOKEN_ROW 0
# define TOP_CARD_ROW 1
# define MID_CARD_ROW 2
# define BOT_CARD_ROW 3
# define ROW_CARD_COUNT 4

void render_board_screen(Context *ctx)
{
	for (int x = 0; x < ROW_COUNT; x++)
	{
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			// if (ctx->board.rows[x].revealed[i] != NULL)
			// {
				// if (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYUP)
				// 	ctx->state = 1;
				SDL_SetRenderDrawColor(ctx->display->renderer,
								255 * (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYHELD),
								255 * (ctx->board.rows[x].cardButton[i].state == SDLX_FOCUS_STAY),
								255,
								255);
				SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].cardButton[i].boundingBox);
			// }
			// SDL_Log(" %d %d | %d %d",
			// 	ctx->board.rows[x].cardButton[i].boundingBox->x,
			// 	ctx->board.rows[x].cardButton[i].boundingBox->y,
			// 	ctx->board.rows[x].cardButton[i].boundingBox->w,
			// 	ctx->board.rows[x].cardButton[i].boundingBox->h
			// );
		}
	}

	// for (int i = 0; i < MAX_ROWCARD; i++)
	// {
	// 	// if (ctx->board.rows[x].revealed[i] != NULL)
	// 	// {

	// 		SDL_SetRenderDrawColor(ctx->display->renderer,
	// 						255 * (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYHELD),
	// 						255 * (ctx->board.rows[x].cardButton[i].state == SDLX_FOCUS_STAY),
	// 						255,
	// 						255);
	// 		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].cardButton[i].boundingBox);
	// 	// }
	// }
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}

void board_screen(Context *ctx)
{
	render_board_screen(ctx);
}

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

	root = initUI("assets/UI/boardUI");

	// initTokens(&ctx->board.rows[TOKEN_ROW], &root->containers[TOKEN_ROW]);
	initRowCards(&ctx->board.rows[TOP_ROW], &root->containers[0]);
	initRowCards(&ctx->board.rows[MID_ROW], &root->containers[1]);
	initRowCards(&ctx->board.rows[BOT_ROW], &root->containers[2]);
}



