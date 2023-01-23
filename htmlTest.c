#include "SDL2/SDL.h"
#include <emscripten.h>
SDL_Window *window;
SDL_Renderer *renderer;

int loop()
[

]

int main(int argc, char* args[])
{
	if (!SDL_WasInit(0))
		SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow(
        name,
        x, y,
        w, h,
        flags
	);
    window = window;
    renderer = SDL_CreateRenderer(display.window, -1, 0);

	emscripten_set_main_loop(loop, 0, 1);
    return 0;
}
