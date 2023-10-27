#include "../includes/splendor.h"
#include "../includes/table.h"

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


SDL_Texture *create_target_texture(int w, int h)
{
	SDL_Texture *result = SDL_CreateTexture(
			SDLX_DisplayGet()->renderer,
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetTextureBlendMode(result , SDL_BLENDMODE_BLEND);

		return result;
}

void reset_game(Context *ctx)
{
	for (int i = 0; i < MAX_POTIONS; ++i)
		ctx->player.owned[i].fill = 0;

	ctx->mainUI.overlay.selected = NULL;
	ctx->board.overlay.selected = NULL;
	ctx->player.actionsRemaining = 0;
	ctx->player.id = 0;
	ctx->player.ownedCount = 0;
	ctx->state = CONNECT;
	set_main_cards_active(ctx, SDL_FALSE);
	set_board_cards_active(ctx, SDL_FALSE);
}

void generatePotion(Potion *card)
{
	int offset;

	offset = 0;
	offset += extract_num(card->id + offset, &card->type);
	offset += extract_num(card->id + offset, &card->fill);

	overlay_text(card->sprite.texture, NULL, NULL, WHITE, 0.6, potions_by_id[card->type].name);

	SDL_Renderer *renderer;
	SDL_Texture *renderTarget;
	SDL_Rect bounds = {0};
	uint32_t color;
	char cost[2] = {"00"};

	renderer = SDLX_DisplayGet()->renderer;
	renderTarget = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, card->sprite.texture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	// SDL_RenderClear(renderer);
	SDL_GetRendererOutputSize(renderer, &bounds.w, &bounds.h);

	bounds.y = bounds.h - (bounds.h / 5);
	bounds.w /= 5;
	bounds.h /= 5;
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		offset += extract_num(card->id + offset, &card->cost[i]);
		color = ((0xFF000000 >> (5 * i)) + 0xFF);
		SDL_itoa(card->cost[i], cost, 10);
		SDLX_RenderMessage(SDLX_DisplayGet(), &bounds, (SDL_Color){
												.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
												.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
												.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
												.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
												, cost);
		bounds.x += bounds.w;

	}
	SDL_SetRenderTarget(renderer, renderTarget);

}



// int extract_card_from_input(Context *ctx, Potion *dst, char *input)
// {
// 	int _id;

// 	memcpy(dst->id, input, CARD_ID_LEN);
// 	extract_num(dst->id, &_id);
// 	if (dst->_id != _id)
// 	{
// 		fillPotion(dst);
// 		// generatePotionTexture(ctx->cardTex, dst, dst->id[1] - '0');
// 	}
// 	else
// 		return 0;
// 	return 1;
// }

// void get_img_src(SDL_Rect *dst, int imageType, int index)
// {
// 	if (index < 0 || index >= POTION_TYPES)
// 		return ;
// 	if (imageType == CARD_BACK)
// 	{
// 		dst->w = CARD_W;
// 		dst->h = CARD_H;
// 		dst->x = OFF_X + (index * (CARD_W + SEP_X));
// 		dst->y = OFF_Y;
// 	}
// 	else if (imageType == CARD)
// 	{
// 		dst->w = CARD_W;
// 		dst->h = CARD_H;
// 		dst->x = OFF_X + (index * (CARD_W + SEP_X));
// 		dst->y = OFF_Y + CARD_H + SEP_Y;
// 	}
// 	else if (imageType == TOK_HEX)
// 	{
// 		dst->w = TOK_W;
// 		dst->h = TOK_H;
// 		dst->x = OFF_X + (POTION_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_OFF_X;
// 		dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
// 	}
// 	else if (imageType == TOK_RECT)
// 	{
// 		dst->w = TOK_W;
// 		dst->h = TOK_H;
// 		dst->x = OFF_X + (POTION_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_W + (TOK_OFF_X * 2);
// 		dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
// 	}
// }
