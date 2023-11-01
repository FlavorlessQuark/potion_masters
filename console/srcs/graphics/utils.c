#include "splendor_structs.h"

/*
	Potion ID

	- tier, effect, components[],
 */

SDL_Rect scale_and_center(double scalar, SDL_Rect parent, SDL_Rect this)
{
	SDL_Rect result;

	result.w = this.w * scalar;
	result.h = this.h * scalar;
	result.x = parent.x + ((parent.w / 2) - (result.w / 2));
	result.y = parent.y + ((parent.h / 2) - (result.h / 2));

	return result;
}


void overlay_text(SDL_Texture *dest, SDL_Texture *base, SDL_Rect *baseSrc,  uint32_t color, char *text)
{
	SDL_Renderer *renderer;
	SDL_Texture *renderTarget;
	SDL_Rect bounds = {0};

	renderer = SDLX_DisplayGet()->renderer;
	renderTarget = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, dest);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_RenderClear(renderer);
	SDL_GetRendererOutputSize(renderer, &bounds.w, &bounds.h);
	if (base)
	{
		SDL_RenderCopy(renderer, base, baseSrc, NULL);
	}

	bounds = scale_and_center(0.6, bounds, bounds);

	SDLX_RenderMessage(SDLX_DisplayGet(), &bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, text);
	SDL_SetRenderTarget(renderer, renderTarget);
}


SDL_Texture *create_target_texture(int w, int h)
{
	SDL_Texture *result = SDL_CreateTexture(
			SDLX_DisplayGet()->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetTextureBlendMode(result , SDL_BLENDMODE_BLEND);

		return result;
}

void draw_player_essences(Context *ctx, Player *player)
{
	char count[2] = {"00"};
	SDL_Rect src = {.x = 20, .y = 0, .w = 260, .h = 360};

	for (int i = 0; i < ESSENCE_TYPES; i++)
	{
		SDL_itoa(player->tokens[i], count, 10);
		overlay_text(player->essences[i].texture, ctx->assets.essences, &src, 0x000000FF, count);
		if (i == 1)
		{
			src.x = 20;
			src.y += src.h + 10;
		}
		else
			src.x += src.w;

	}
}
