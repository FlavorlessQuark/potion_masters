#include "../includes/splendor.h"

void board_screen(Context *ctx)
{
	if (ctx->board.switchMode.triggered == SDLX_KEYDOWN)
		ctx->state = 0;
	render_board_screen(ctx);
}


void render_board_screen(Context *ctx)
{
	for (int x = 0; x < ROW_COUNT; x++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].rowIcon.dst);
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			if (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYDOWN)
			{
				ctx->state = 2;
				ctx->buyscreen->cardOrigin = 0;
			}
			SDL_SetRenderDrawColor(ctx->display->renderer,
							255 * (ctx->board.rows[x].cardButton[i].triggered == SDLX_KEYHELD),
							255 * (ctx->board.rows[x].cardButton[i].state == SDLX_FOCUS_STAY),
							255,
							255);
			SDL_RenderDrawRect(ctx->display->renderer, ctx->board.rows[x].cardButton[i].boundingBox);
		}
	}

	for (int i = 0; i < TOK_COUNT + 2; i++)
	{
		SDL_SetRenderDrawColor(ctx->display->renderer,
						255 * (ctx->board.tokenButton[i].triggered == SDLX_KEYHELD),
						255 * (ctx->board.tokenButton[i].state == SDLX_FOCUS_STAY),
						255,
						255);
		SDL_RenderDrawRect(ctx->display->renderer, ctx->board.tokenButton[i].boundingBox);
	}

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.switchMode.boundingBox);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
}
