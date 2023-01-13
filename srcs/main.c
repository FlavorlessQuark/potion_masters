#include "../includes/splendor.h"

int main()
{
	Context *ctx;

	ctx = init();
	while (1)
	{
		SDLX_RenderReset(ctx->display);
		SDLX_InputLoop();
		SDLX_InputUpdate();
		SDLX_ButtonUpdate();
		// SDLX_TimedLoop(&core, ctx);
		core(ctx);

		SDL_RenderPresent(ctx->display->renderer);
	}
}
