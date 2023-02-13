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
		core(ctx, msg);
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
