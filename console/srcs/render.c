#include "../includes/splendor.h"


void renderBoard(Context *ctx)
{
	for (int x = 0; x < ROW_COUNT; x++)
	{
		if (ctx->board.rows[x].remainCount > 0)
		{
			SDL_SetRenderDrawColor(ctx->display->renderer,
								255,
								255,
								255,
								255);
			SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].rowIcon.dst);
		}
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			if (ctx->board.rows[x].revealed[i] != NULL)
			{

				SDL_SetRenderDrawColor(ctx->display->renderer,
								255 * (ctx->board.rows[x].cardButton[i].triggered),
								255 * (ctx->board.rows[x].cardButton[i].state == SDLX_FOCUS_STAY),
								255,
								255);
				SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].cardButton[i].boundingBox);
			}
		}
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


void renderConnectScreen(Context *ctx, uint8_t ready[MAX_PLAYERS])
{
	int space;
	SDL_Rect rect;

	rect.h = ctx->display->win_h / 3;
	rect.w = ctx->display->win_w / 5;
	rect.y = ctx->display->win_h / 3;
	rect.x = ctx->display->win_w / 10;
	space = rect.x;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (i > ctx->playerCount)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
		else if (ready[i])
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 255, 0x0,255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 255, 255);
		SDL_RenderDrawRect(ctx->display->renderer, &rect);
		rect.x += space;
	}
}
