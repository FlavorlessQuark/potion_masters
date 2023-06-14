#include "splendor_structs.h"

SDL_Rect scaleAndCenter(double scalar, SDL_Rect parent, SDL_Rect this)
{
	SDL_Rect result;

	result.w = this.w * scalar;
	result.h = this.h * scalar;
	result.x = parent.x + ((parent.w / 2) - (result.w / 2));
	result.y = parent.y + ((parent.h / 2) - (result.h / 2));

	return result;
}

void generatePotionTexture(SDL_Texture *base, Potion *card, int type)
{
	SDL_Rect src;
	SDL_Rect dst;
	SDL_Rect centereDst;
	SDL_Renderer *renderer;
	SDL_Texture *target;
	char text[2];
	int remainder;

	text[1] = '\0';
	renderer = SDLX_DisplayGet()->renderer;
	target = SDL_GetRenderTarget(renderer);
	// get_img_src(&src, CARD, type);

	dst.h = card->sprite.dst->h / 7;
	dst.w = dst.h;
	dst.x = card->sprite.dst->w / 10;
	dst.y = card->sprite.dst->h / 10;


	if (card->sprite.texture == NULL)
	{
		SDL_Log(" !! New card (%d) has no texture", card->_id);
		card->sprite.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, card->sprite._dst.w, card->sprite._dst.h);
		SDL_SetTextureBlendMode(card->sprite.texture , SDL_BLENDMODE_BLEND);
	}


	SDL_SetRenderTarget(renderer, card->sprite.texture);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderCopy(renderer, base, &src, NULL);
	SDL_Log("Texture fro card %s", card->id);
	for (int i = 0; i < POTION_TYPES; i++)
	{
		if (card->cost[i] > 0)
		{
			// get_img_src(&src, TOK_HEX, i);

			centereDst = scaleAndCenter(0.5, dst, dst);
			// SDL_Log("Token %d x%d  (%s)", i, card->);
			SDL_RenderCopy(renderer, base, &src, &dst);
			remainder = card->cost[i];
			if (remainder >= 5)
			{
				remainder -= 5;
				SDLX_RenderMessage(SDLX_DisplayGet(), &centereDst,(SDL_Color){0,0,0,0}, "5");
				dst.x += dst.w / 5;
			}
			for (int x = 0; x < remainder; x++)
			{
				SDL_RenderCopy(renderer, base, &src, &dst);
				dst.x += dst.w / 5;
			}
			// SDLX_RenderMessage(SDLX_DisplayGet(), &centereDst,(SDL_Color){255,255,255,255}, text);
			dst.y += card->sprite.dst->h / 5;
		}
		dst.x = card->sprite.dst->w / 10;
	}
	dst.x = card->sprite._dst.w * 0.70;
	dst.y = dst.y = card->sprite.dst->h / 10;
	text[0] = card->points + '0';
	SDLX_RenderMessage(SDLX_DisplayGet(), &dst,(SDL_Color){255,255,255,255}, text);
	SDL_SetRenderTarget(renderer, target);
}
