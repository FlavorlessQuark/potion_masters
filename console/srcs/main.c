#include "../includes/splendor.h"

void cleanup_game(Context *ctx);

int main()
{
	Context *ctx;
	char *msg;

	ctx = init();
	while (ctx->state != EXIT_GAME)
	{
		SDLX_RenderReset(ctx->display);
		SDLX_RenderQueueFlushAll();
		input_loop(ctx);
		core(ctx, msg);
		SDLX_RenderResetColour(ctx->display);
		SDLX_RenderAll(ctx->display);
		SDL_RenderPresent(ctx->display->renderer);
	}
	cleanup_game(ctx);
}


void cleanup_game(Context *ctx)
{
	SDL_DestroyTexture(ctx->Tcards);
	SDL_DestroyTexture(ctx->Tbuttons);
	SDL_DestroyTexture(ctx->textSheet.tex);
	for (int n = 0; n < ROW_COUNT; n++)
	{
		for (int i = 0; i < MAX_ROWCARD; i++)
		{
			SDL_DestroyTexture(ctx->board.rows[i].revealed[i].sprite.texture);
		}
	}
	for (int n = 0; n < MAX_PLAYERS; n++)
	{
		for (int i = 0; i < MAX_RESERVE; i++)
		{
			SDL_DestroyTexture(ctx->players[n].reserved[i].sprite.texture);
		}
	}
}
