#include "../includes/splendor.h"

int checkRow();



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
	}
	for (int i = 0; i < ctx->players[0].reserveCount; i++)
	{
		// if (ctx->board.rows[0].revealed[i] != NULL)
		// // {
			// SDL_Log("BOX %d,%d } %d %d",
			// 	 ctx->players[0].reservedButton[i].boundingBox->x,
			// 	 ctx->players[0].reservedButton[i].boundingBox->y,
			// 	 ctx->players[0].reservedButton[i].boundingBox->h,
			// 	 ctx->players[0].reservedButton[i].boundingBox->w
			//  );

			SDL_SetRenderDrawColor(ctx->display->renderer,
							   255 * (ctx->players[0].reservedButton[i].triggered),
							   255 * (ctx->players[0].reservedButton[i].state == SDLX_FOCUS_STAY),
							   255,
							   255);
			SDL_RenderDrawRect(ctx->display->renderer, ctx->players[0].reservedButton[i].boundingBox);
		// }
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
