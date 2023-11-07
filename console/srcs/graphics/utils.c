#include "splendor_structs.h"
#include "table.h"
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

void get_render_texture_dimensions(SDL_Texture *texture, int *w, int *h)
{
	SDL_Renderer *renderer;
	SDL_Texture *renderTarget;

	renderer = SDLX_DisplayGet()->renderer;
	renderTarget = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_GetRendererOutputSize(renderer, w, h);
	SDL_SetRenderTarget(renderer, renderTarget);
}

void overlay_texture(SDL_Texture *to, SDL_Texture *from, SDL_Rect *fromSrc, SDL_Rect *dst)
{
}

void overlay_text(SDL_Texture *dest, SDL_Texture *base, SDL_Rect *baseSrc, uint32_t color, char *text)
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

	SDLX_RenderMessage(SDLX_DisplayGet(), &bounds, (SDL_Color){.r = (color & ((uint32_t)(0xFF << 24))) >> 24, .g = (color & ((uint32_t)(0xFF << 16))) >> 16, .b = (color & ((uint32_t)(0xFF << 8))) >> 8, .a = (color & ((uint32_t)(0xFF << 0))) >> 0}, text);
	SDL_SetRenderTarget(renderer, renderTarget);
}

SDL_Texture *create_target_texture(int w, int h)
{
	SDL_Texture *result = SDL_CreateTexture(
		SDLX_DisplayGet()->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);

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

#define POTION_PER_ROW (5)
#define HORZ_SEP (10)
#define VERT_SEP (10)

void generate_potion_texture(Context *ctx, Potion *card)
{
	SDL_Rect potion_src = {0};
	SDL_Rect essence_src = {0};
	SDL_Rect potion_dst = {0};
	SDL_Rect bounds = {0};
	SDL_Rect scaled_bounds;

	potion_src.w = 370;
	potion_src.h = 540;
	potion_src.y = (card->type / POTION_PER_ROW) * (potion_src.h + VERT_SEP);
	potion_src.x = (card->type % POTION_PER_ROW) * (potion_src.w + HORZ_SEP);

	get_render_texture_dimensions(card->sprite.texture, &bounds.w, &bounds.h);
	potion_dst = scale_and_center(0.7, bounds, bounds);

	SDL_SetRenderTarget(ctx->display->renderer, card->sprite.texture);
	SDL_RenderClear(ctx->display->renderer);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 0);
	SDL_RenderCopy(ctx->display->renderer, ctx->assets.recipeBg, NULL, NULL);
	SDL_RenderCopy(ctx->display->renderer, ctx->assets.texPotions, &potion_src, &potion_dst);

	uint32_t color = 0x000000FF;
	char cost[2] = {"00"};

	bounds.w /= 7;
	bounds.y = bounds.h - bounds.w - 10;
	bounds.h = bounds.w;
	bounds.x += bounds.w;
	essence_src = (SDL_Rect){.x = 20, .y = 0, .w = 260, .h = 360};
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		if (card->cost[i] > 0)
		{

			SDL_itoa(card->cost[i], cost, 10);
			SDL_RenderCopy(ctx->display->renderer, ctx->assets.essences, &essence_src, &bounds);
			scaled_bounds = scale_and_center(0.55, bounds, bounds);
			SDLX_RenderMessage(SDLX_DisplayGet(), &scaled_bounds, (SDL_Color){.r = (color & ((uint32_t)(0xFF << 24))) >> 24, .g = (color & ((uint32_t)(0xFF << 16))) >> 16, .b = (color & ((uint32_t)(0xFF << 8))) >> 8, .a = (color & ((uint32_t)(0xFF << 0))) >> 0}, cost);
			bounds.x += bounds.w + 10;
		}
		if (i == 1)
		{
			essence_src.x = 20;
			essence_src.y += essence_src.h + 10;
		}
		else
			essence_src.x += essence_src.w;
	}

	bounds.y = bounds.h / 7;
	for (int i = 0; i < potions_by_id[card->type].cost_count; ++i)
	{
		potion_src.y = (potions_by_id[card->type].potion_cost[i] / POTION_PER_ROW) * (potion_src.h + VERT_SEP);
		potion_src.x = (potions_by_id[card->type].potion_cost[i] % POTION_PER_ROW) * (potion_src.w + HORZ_SEP);
		SDL_RenderCopy(ctx->display->renderer, ctx->assets.texPotions, &potion_src,
		&(SDL_Rect){.x = bounds.w * 5.5, .y = bounds.y + (bounds.h / 7), .w = bounds.w, .h = bounds.h});
		bounds.y += bounds.h;
		}


	SDL_SetRenderTarget(ctx->display->renderer, NULL);
}

