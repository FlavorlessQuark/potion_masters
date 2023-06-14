#include "../includes/splendor.h"

int main()
{
	Context *ctx;
	char *msg;

	ctx = init();
	// execMsg(ctx, "0p0|000000");
	while (1)
	{
		SDLX_RenderReset(ctx->display);
		SDLX_RenderQueueFlushAll();
		SDLX_InputLoop();
		core(ctx, msg);
		SDLX_RenderResetColour(ctx->display);
		SDLX_RenderAll(ctx->display);
		SDL_RenderPresent(ctx->display->renderer);
	}
}
