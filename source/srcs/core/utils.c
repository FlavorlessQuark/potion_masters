#include "game.h"
#include "table.h"

#define NUMS "0123456789"

static int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}

SDL_Rect scale_and_center(double scalar, SDL_Rect parent, SDL_Rect this)
{
	SDL_Rect result;

	result.w = this.w * scalar;
	result.h = this.h * scalar;
	result.x = parent.x + ((parent.w / 2) - (result.w / 2));
	result.y = parent.y + ((parent.h / 2) - (result.h / 2));

	return result;
}

void overlay_text(SDL_Texture *dest, SDL_Texture *base, SDL_Rect *baseSrc, uint32_t color, double scale, char *text)
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

	if (scale != 1)
		bounds = scale_and_center(0.6, bounds, bounds);

	SDLX_RenderMessage(SDLX_DisplayGet(), &bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, text);

	SDL_SetRenderTarget(renderer, renderTarget);
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

SDL_Texture *create_target_texture(int w, int h)
{
	SDL_Texture *result = SDL_CreateTexture(
			SDLX_DisplayGet()->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetTextureBlendMode(result , SDL_BLENDMODE_BLEND);

		return result;
}


#define POTION_PER_ROW (5)
#define HORZ_SEP (10)
#define VERT_SEP (10)

void overlay_texture(SDL_Texture *to, SDL_Texture *from, SDL_Rect *dst, SDL_Rect *src)
{
	SDL_Renderer *renderer;
	SDL_Texture *renderTarget;

	renderer = SDLX_DisplayGet()->renderer;
	renderTarget = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, to);
	SDL_RenderCopy(renderer, from, src, dst);
	SDL_SetRenderTarget(renderer, renderTarget);
}

void generate_potion(Context *ctx, Potion *card, int isOwned)
{
	int offset;
	SDL_Rect potion_src = {0};
	SDL_Rect essence_src = {0};
	SDL_Rect potion_dst = {0};
	SDL_Rect bounds = {0};
	SDL_Rect scaled_bounds;
	uint32_t color = 0x000000FF;
	char cost[2] = {"00"};

	offset = 0;
	offset += extract_num(card->id + offset, &card->type);
	offset += extract_num(card->id + offset, &card->fill);

	potion_src.w = 370;
	potion_src.h = 540;
	potion_src.y = (card->type / POTION_PER_ROW) * (potion_src.h + VERT_SEP);
	potion_src.x = (card->type % POTION_PER_ROW) * (potion_src.w + HORZ_SEP);

	get_render_texture_dimensions(card->sprite.texture, &bounds.w, &bounds.h);

	SDL_SetRenderTarget(ctx->display->renderer, card->sprite.texture);
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 0);
	SDL_RenderClear(ctx->display->renderer);
	// SDL_RenderCopy(ctx->display->renderer, ctx->assets.recipeBg, NULL, NULL);
	if (!isOwned)
	{
		potion_dst = scale_and_center(0.6, bounds, bounds);
		potion_dst.y -= potion_dst.h / 15;
		SDL_RenderCopy(ctx->display->renderer, ctx->assets.recipe, NULL, NULL);
		SDL_RenderCopy(ctx->display->renderer, ctx->assets.potions, &potion_src, &potion_dst);
	}
	else
		SDL_RenderCopy(ctx->display->renderer, ctx->assets.potions, &potion_src, NULL);


	if (!isOwned)
	{
		bounds.w /= 7;
		bounds.y = bounds.h - bounds.w - 10;
		bounds.h = bounds.w;
		bounds.x += bounds.w;
		essence_src = (SDL_Rect){.x = 20, .y = 0, .w = 260, .h = 360};
		for (int i = 0; i < ESSENCE_TYPES; ++i)
		{
			offset += extract_num(card->id + offset, &card->cost[i]);
			SDL_itoa(card->cost[i], cost, 10);
			// // SDL_Log(" COST %d", card->cost[i]);
			if (card->cost[i] > 0)
			{
				// SDL_RenderCopy(ctx->display->renderer, ctx->assets.essence, &essence_src, &bounds);
				SDL_RenderCopy(ctx->display->renderer, ctx->assets.essences, &essence_src, &bounds);
				scaled_bounds = scale_and_center(0.55, bounds, bounds);
				SDLX_RenderMessage(SDLX_DisplayGet(), &scaled_bounds, (SDL_Color){
														.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
														.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
														.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
														.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
														, cost);
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
			SDL_RenderCopy(ctx->display->renderer, ctx->assets.potions, &potion_src,
			&(SDL_Rect){.x = bounds.w * 5.5, .y = bounds.y + (bounds.h / 7), .w = bounds.w, .h = bounds.h});
			bounds.y += bounds.h;
		}
	}

	SDL_SetRenderTarget(ctx->display->renderer, NULL);

}

int can_buy_potion(Context *ctx, Potion *potion)
{
	int x;

	if (ctx->player.isBrewing)
		return SDL_FALSE;
	if (ctx->player.actionsRemaining <= 0)
		return SDL_FALSE;
	if (ctx->player.ownedCount >= MAX_POTIONS)
		return SDL_FALSE;
	for (int i = 0; i < potions_by_id[potion->type].cost_count; ++i)
	{
		for (x = 0; x < ctx->player.ownedCount; ++x)
		{
			// // SDL_Log("At %d %d Need potion %d", i, ctx->player.owned[i].type, potions_by_id[potion->type].potion_cost[i]);
			if (ctx->player.owned[x].type == potions_by_id[potion->type].potion_cost[i])
				break ;
		}
		if (x == ctx->player.ownedCount)
		{
			return SDL_FALSE;
		}
	}
	for (int i = 0; i < ESSENCE_TYPES ; ++i)
	{
		if (ctx->player.tokens[i] < potion->cost[i])
			return SDL_FALSE;
	}


	return SDL_TRUE;
}

#define POTION_PER_ROW (5)
#define HORZ_SEP (10)
#define VERT_SEP (10)

void copy_potion(Potion *dst, Potion *src)
{
	dst->fill = src->fill;
	dst->type = src->type;
	SDL_memcpy(dst->id ,src->id, CARD_ID_LEN);
	SDL_memcpy(dst->cost ,src->cost, ESSENCE_TYPES * sizeof(int));
}
