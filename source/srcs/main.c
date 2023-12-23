#include "../includes/game.h"

int main()
{
	Context *ctx;
	char *msg;

	ctx = init();

	while (1)
	{
		SDLX_RenderReset(ctx->display);
		SDLX_RenderQueueFlushAll();
		SDLX_InputLoop();
		core(ctx, msg);
		SDLX_RenderAll(ctx->display);
		// SDLX_RenderResetColour(ctx->display);
		SDL_RenderPresent(ctx->display->renderer);
	}
}
