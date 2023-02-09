#include "../includes/splendor.h"


void renderBoard(Context *ctx)
{
	int i;

	SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0,0,255);

	SDL_RenderDrawRect(ctx->display->renderer, &ctx->board.playerUI[0].nameTag);
	SDL_RenderDrawRect(ctx->display->renderer, &ctx->board.playerUI[0].pointsTag);

	// for (i = 0; i < CARD_TYPES; i++)
	// {
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].ressourceIcon[i].dst);
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].permanentIcon[i].dst);
	// }
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].ressourceIcon[i].dst);


	// for (i = 0; i < MAX_RESERVE; i++)
	// {
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].reservedIcon[i].dst);
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].reservedPrice[i * (TOK_COUNT - 1) + 0].dst);
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].reservedPrice[i * (TOK_COUNT - 1) + 1].dst);
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].reservedPrice[i * (TOK_COUNT - 1) + 2].dst);
	// 	SDL_RenderDrawRect(ctx->display->renderer, ctx->board.playerUI[0].reservedPrice[i * (TOK_COUNT - 1) + 3].dst);
	// }
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0,0,255);
}


