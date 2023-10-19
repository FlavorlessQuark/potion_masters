#include "../includes/splendor.h"
#define NUMS "0123456789"


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

void reset_game(Context *ctx)
{
	char cost[2] = {"00"};
	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		ctx->player.tokens[i] = 3;
		SDL_itoa(3, cost, 10);
		overlay_text(ctx->mainUI.essences[i].texture, NULL, NULL, ((0xFF000000 >> (5 * i)) + 0xFF), cost);
	}
	for (int i = 0; i < MAX_POTIONS; ++i)
		ctx->player.owned[i].fill = 0;

	ctx->player.actionsRemaining = 0;
	ctx->player.id = 0;
	ctx->player.ownedCount = 0;
}

// TODO: Update this (very old, do not use)
// void generatePotionTexture(SDL_Texture *base, Potion *card, int type)
// {
// 	SDL_Rect src;
// 	SDL_Rect dst;
// 	SDL_Rect centereDst;
// 	SDL_Renderer *renderer;
// 	char text[2];
// 	int remainder;

// 	text[1] = '\0';
// 	renderer = SDLX_DisplayGet()->renderer;
// 	get_img_src(&src, CARD, type);

// 	dst.h = card->sprite.dst->h / 7;
// 	dst.w = dst.h;
// 	dst.x = card->sprite.dst->w / 10;
// 	dst.y = card->sprite.dst->h / 10;
// 	SDL_SetRenderTarget(renderer, card->sprite.texture);
// 	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
// 	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
// 	SDL_RenderFillRect(renderer, NULL);
// 	SDL_RenderCopy(renderer, base, &src, NULL);
// 	for (int i = 0; i < POTION_TYPES; i++)
// 	{
// 		if (card->cost[i] > 0)
// 		{
// 			get_img_src(&src, TOK_HEX, i);

// 			centereDst = scale_and_center(0.5, dst, dst);
// 			// SDL_Log("Token %d x%d  (%s)", i, card->);
// 			SDL_RenderCopy(renderer, base, &src, &dst);
// 			remainder = card->cost[i];
// 			if (remainder >= 5)
// 			{
// 				remainder -= 5;
// 				SDLX_RenderMessage(SDLX_DisplayGet(), &centereDst,(SDL_Color){0,0,0,0}, "5");
// 				dst.x += dst.w / 5;
// 			}
// 			for (int x = 0; x < remainder; x++)
// 			{
// 				SDL_RenderCopy(renderer, base, &src, &dst);
// 				dst.x += dst.w / 5;
// 			}
// 			dst.y += card->sprite.dst->h / 5;
// 		}
// 		dst.x = card->sprite.dst->w / 10;
// 	}
// 	dst.x = card->sprite._dst.w * 0.70;
// 	dst.y = dst.y = card->sprite.dst->h / 10;
// 	text[0] = card->points + '0';
// 	SDLX_RenderMessage(SDLX_DisplayGet(), &dst,(SDL_Color){255,255,255,255}, text);
// 	SDL_SetRenderTarget(renderer, NULL);
// }

// void fillPotion(Potion *card)
// {
// 	char *str;
// 	int type;
// 	int i;

// 	type = card->id[1] - '0';
// 	extract_num(card->id, &card->_id);
// 	if (card->_id <= 0)
// 		return ;
// 	get_img_src(&card->sprite._src, CARD, type);
// 	str = card->id + 3;
// 	for (i = 0; i < POTION_TYPES + 1; i++)
// 		card->cost[i] = str[i] - '0';

// 	card->points = str[i] - '0';
// 	SDL_Log("Generate %s (%d) | Src (%d,%d) Cost: %d %d %d %d",
// 		card->id, card->_id,
// 		card->sprite._src.x, card->sprite._src.y,
// 		card->cost[0],
// 		card->cost[1],
// 		card->cost[2],
// 		card->cost[3]
// 	);
// }

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
