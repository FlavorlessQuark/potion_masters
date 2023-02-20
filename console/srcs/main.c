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
		SDLX_RenderAll(ctx->display);
		SDL_RenderPresent(ctx->display->renderer);
	}
}
// int main()
// {
// 	Context *ctx;
// 	char *msg;

// 	ctx = init();
// 	while (1)
// 	{
// 		// SDLX_RenderReset(ctx->display);
// 		SDLX_InputLoop();
// 		// SDLX_InputUpdate();
// 		// SDLX_ButtonUpdate();
// 		// msg = input(ctx);
// 		// decodeMsg(ctx, msg);
// 		// core(ctx, msg);

// 		SDL_RenderPresent(ctx->display->renderer);
// 	}
// }
