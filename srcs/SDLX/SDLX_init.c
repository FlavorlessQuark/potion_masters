#include "SDLX/SDLX.h"

static	SDLX_Display display;
void 	SDLX_BackgroundSet(SDL_Texture *bg) {display.background = bg;}

void	SDLX_Close(void)
{
	SDL_DestroyWindow(display.window);
    TTF_CloseFont(display.defaultFont);

    TTF_Quit();
    IMG_Quit();
	SDL_Quit();
}

static void		SDLX_DisplaySet(char *name, int x, int y, int h, int w, int flags)
{
    SDL_Window *window;

	window = SDL_CreateWindow(
        name,
        x, y,
        w, h,
        flags
	);
    display.window = window;
    display.renderer = SDL_CreateRenderer(display.window, -1, 0);
    display.win_w = w;
    display.win_h = h;
}

void SDLX_InitDefault()
{
	if (!SDL_WasInit(0))
		SDL_Init(SDL_INIT_EVERYTHING);
    if (!TTF_WasInit() && !TTF_Init())
    {
        SDL_Log("TTF initialized");
        display.defaultFont = TTF_OpenFont(DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE);
    }

	SDLX_DisplaySet(
		DEFAULT_WIN_NAME,
        DEFAULT_WIN_X,
		DEFAULT_WIN_Y,
        DEFAULT_WIN_H,
		DEFAULT_WIN_W,
        DEFAULT_SDL_FLAG
	);
    SDLX_RenderQueuesInit();
	atexit(SDLX_Close);
}

void	SDLX_Init(char *name, int x, int y, int h, int w, int flags)
{
	if (!SDL_WasInit(0))
		SDL_Init(SDL_INIT_EVERYTHING);
    if (!TTF_WasInit() && !TTF_Init())
    {
        SDL_Log("TTF initialized");
        display.defaultFont = TTF_OpenFont(DEFAULT_FONT_PATH, DEFAULT_FONT_SIZE);
    }

	SDLX_DisplaySet(name, x, y, h, w, flags);
    SDLX_RenderQueuesInit();
	atexit(SDLX_Close);
}

SDLX_Display	*SDLX_DisplayGet(void)
{
    if (!display.renderer)
        return NULL;

    return &display;
}
