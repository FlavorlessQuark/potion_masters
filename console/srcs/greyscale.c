// #include "../includes/SDLX/SDLX.h"

// int main()
// {
// 	SDLX_Display *display;
// 	SDL_Texture *tex;

// 	SDLX_InitDefault();

// 	SDL_BlendFactor f;
// 	display = SDLX_DisplayGet();
// 	tex = SDLX_TextureLoad("assets/Textures/essences.png", display);
// 	SDL_CreateTexture(display->renderer, )
// 	SDL_SetRenderDrawBlendMode(display->renderer, SDL_BLENDMODE_ADD);
// 	SDL_Surface *surf = SDL_CreateRGBSurfaceFrom();
// 	while (1)
// 	{
// 		SDLX_RenderReset(display);
// 		SDLX_InputLoop();
// 		SDL_RenderCopy(display->renderer, tex, NULL, NULL);
// 		SDL_SetRenderDrawColor(display->renderer, 100,100,100,200);
// 		SDL_RenderFillRect(display->renderer, NULL);
// 		SDL_SetRenderDrawColor(display->renderer, 0,0,0,255);
// 		SDL_RenderPresent(display->renderer);
// 	}
// }
