#include "../includes/splendor.h"

int main()
{
	Context *ctx;
	char *msg;
	ctx = init();
	while (1)
	{
		SDLX_RenderReset(ctx->display);
		SDLX_InputLoop();
		SDLX_InputUpdate();
		SDLX_ButtonUpdate();
		SDLX_TimedLoop(&core, ctx);
		msg = input(ctx);
		core(ctx, msg);

		SDL_RenderPresent(ctx->display->renderer);
	}
}
