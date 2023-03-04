#include "../../includes/SDLX/SDLX.h"

SDLX_TextSheet SDLX_TextSheet_Create(SDLX_TextSheet *dst, int maxW, int maxH)
{
	SDLX_Display *display;

	display = SDLX_DisplayGet();
	dst->tex = SDL_CreateTexture(display->renderer,
		SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_TARGET,
		maxW, maxH
	);

	dst->maxW = maxW;
	dst->maxH = maxH;
	dst->next.h = 0;
	dst->next.w = 0;
	dst->next.x = 0;
	dst->next.y = 0;
}


SDL_Rect SDLX_TextSheet_Add(SDLX_TextSheet *dst, char *text, TTF_Font *font, SDL_Color colour)
{
	SDLX_Display *display;
	SDL_Rect textBox;
	SDL_Surface *surf;
	SDL_Texture *tex;

	display = SDLX_DisplayGet();
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

	surf = TTF_RenderText_Solid(font,text, colour);
	tex = SDL_CreateTextureFromSurface(display->renderer,surf);

	SDL_RenderCopy(display->renderer, tex, NULL, &textBox);
	SDL_SetRenderTarget(display->renderer, NULL);
	return textBox;
}
