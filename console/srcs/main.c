#include "../includes/splendor.h"

int main()
{
	Context *ctx;
	char *msg;

	ctx = init();
	// init_new_game(ctx);
	// execMsg(ctx, "0p0|000000");
	// ctx->display->background = ctx->assets.texPotions;
	// DEBUG("DEBUG TEST HERE");
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
