#include "../includes/splendor.h"

int checkRow();


// msg = "Player|Action"
// Action -> Reserve : r-[card-id]
// 		  -> Pay : p-[card_id]
//        -> Take : t-[tok1, tok2, tok3, tok4, tok5]
int decodeMsg(Context *ctx, char *msg)
{

}

int core(void *arg, char *msg)
{
	Context *ctx;
	static uint8_t score;
	static uint8_t money[5];
	static uint8_t owned[4];

	ctx = (Context *)arg;
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		// if (ctx->board.rows[0].revealed[i] != NULL)
		// {

			SDL_SetRenderDrawColor(ctx->display->renderer,
							   255 * (ctx->board.rows[0].cardButton[i].triggered),
							   255 * (ctx->board.rows[0].cardButton[i].state == SDLX_FOCUS_STAY),
							   255,
							   255);
			SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[0].cardButton[i].boundingBox);
		// }
		if ((ctx->board.rows[0].cardButton[i].triggered))
		{
			SDL_Log("triggered, %d %d %d %d + %d point| Remaining:  | points : %d");
		}
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
