#include "../../includes/SDLX/SDLX.h"

SDLX_TextSheet SDLX_TextSheet_Create(SDLX_TextSheet *dst, int maxW, int maxH)
{
	SDLX_Display *display;
	SDL_Texture	 *renderTarget;

	display = SDLX_DisplayGet();
	dst->tex = SDL_CreateTexture(display->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		maxW, maxH
	);

	renderTarget = SDL_GetRenderTarget(display->renderer);
	SDL_SetRenderTarget(display->renderer, dst->tex);
	SDL_SetRenderDrawBlendMode(display->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(dst->tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(display->renderer, 0, 0, 255, 0);
	SDL_RenderFillRect(display->renderer, NULL);
	SDL_RenderClear(display->renderer);

	dst->maxW = maxW;
	dst->maxH = maxH;
	dst->next.h = 0;
	dst->next.w = 0;
	dst->next.x = 0;
	dst->next.y = 0;

	SDL_SetRenderTarget(display->renderer, renderTarget);

}


SDL_Rect SDLX_TextSheet_Add(SDLX_TextSheet *dst, char *text, TTF_Font *font, SDL_Color colour)
{
	SDLX_Display *display;
	SDL_Rect textBox;
	SDL_Surface *surf;
	SDL_Texture *tex;
	SDL_Texture *renderTarget;

	display = SDLX_DisplayGet();

	renderTarget = SDL_GetRenderTarget(display->renderer);
	SDL_SetRenderDrawBlendMode(display->renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(display->renderer, dst->tex);
	TTF_SizeText(font, text, &textBox.w, &textBox.h);

	if (dst->maxW < dst->next.x + textBox.w)
	{
		dst->next.y += dst->next.h;
		dst->next.h = 0;
		dst->next.x = 0;
	}

	textBox.x = dst->next.x;
	textBox.y = dst->next.y;

	dst->next.x += textBox.w;
	dst->next.h = MAX(textBox.h, textBox.h);

	surf = TTF_RenderText_Blended(font,text, colour);
	tex = SDL_CreateTextureFromSurface(display->renderer,surf);

	// SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(display->renderer, tex, NULL, &textBox);
	SDL_SetRenderTarget(display->renderer, renderTarget);

	return textBox;
}
