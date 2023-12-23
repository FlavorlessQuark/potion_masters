#include "../includes/game.h"
#include "../includes/SDLX/SDLX.h"

Context ctx;

typedef void (*loop)(Context *);

loop core_loops[4] = {
    title_screen,
    lobby_screen,
    main_screen,
    board_screen
    };

void core(void)
{
	SDLX_RenderReset(ctx.display);
	SDLX_RenderQueueFlushAll();
	// window_events(&ctx);
	SDLX_InputUpdate();
	SDLX_ContainerUpdate	(root, NULL);
	//
	SDLX_ButtonUpdate();

	if (ctx.connection.hasMessage == SDL_TRUE)
	{
		// SDL_Log("Received a message %s",  ctx.connection.message);
		parse_message(&ctx, ctx.connection.message);
		ctx.connection.hasMessage = SDL_FALSE;
	}
	core_loops[ctx.state](&ctx);

    // switch (ctx.state)
    // {
    //     case GAME_TITLE:
    //         main_screen(&ctx);
    //         break;

    //     default:
    //         break;
    // }
	// if (ctx.state == GAME_)
	// else if (ctx.state == BOARD)
	// 	board_screen(&ctx);
	// else if (ctx.state == CONNECT)
	// 	connect_screen(&ctx);
	// else if (ctx.state == END)
	// 	connect_screen(&ctx);
	SDLX_RenderAll(ctx.display);
	// SDLX_DisplayConfig(ctx.display->renderer, root);
	// SDL_RenderCopy(ctx.display->renderer, ctx.switchSprite.texture, NULL, NULL);
	SDL_RenderPresent(ctx.display->renderer);
}

int main(int argc, char **argv)
{
	// init_static(&ctx, SDL_atoi(argv[1]), SDL_atoi(argv[2]));
    init_game(&ctx, 1920 / 2 , 1080 / 2);
    // init_static(&ctx, 1920 / 2 , 1080 / 2);

    while(1)
        core();
}
