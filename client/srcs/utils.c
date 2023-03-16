#include "../includes/splendor.h"
#define NUMS "0123456789"


SDLX_RectContainer *parseUI(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}

SDL_Rect scaleAndCenter(double scalar, SDL_Rect parent, SDL_Rect this)
{
	SDL_Rect result;

	result.w = this.w * scalar;
	result.h = this.h * scalar;
	result.x = parent.x + ((parent.w / 2) - (result.w / 2));
	result.y = parent.y + ((parent.h / 2) - (result.h / 2));

	return result;
}

void generateCardTexture(SDL_Texture *base, Card *card, int type)
{
	SDL_Rect src;
	SDL_Rect dst;
	SDL_Rect centereDst;
	SDL_Renderer *renderer;
	char text[2];
	int remainder;

	text[1] = '\0';
	renderer = SDLX_DisplayGet()->renderer;
	get_img_src(&src, CARD, type);

	dst.h = card->sprite.dst->h / 7;
	dst.w = dst.h;
	dst.x = card->sprite.dst->w / 10;
	dst.y = card->sprite.dst->h / 10;
	SDL_SetRenderTarget(renderer, card->sprite.texture);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
	SDL_RenderFillRect(renderer, NULL);
	SDL_RenderCopy(renderer, base, &src, NULL);
	for (int i = 0; i < CARD_TYPES; i++)
	{
		if (card->cost[i] > 0)
		{
			get_img_src(&src, TOK_HEX, i);

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
			dst.y += card->sprite.dst->h / 5;
		}
		dst.x = card->sprite.dst->w / 10;
	}
	dst.x = card->sprite._dst.w * 0.70;
	dst.y = dst.y = card->sprite.dst->h / 10;
	text[0] = card->points + '0';
	SDLX_RenderMessage(SDLX_DisplayGet(), &dst,(SDL_Color){255,255,255,255}, text);
	SDL_SetRenderTarget(renderer, NULL);
}

void fillCard(Card *card)
{
	char *str;
	int type;
	int i;

	type = card->id[1] - '0';
	extract_num(card->id, &card->_id);
	if (card->_id <= 0)
		return ;
	get_img_src(&card->sprite._src, CARD, type);
	str = card->id + 3;
	for (i = 0; i < CARD_TYPES + 1; i++)
		card->cost[i] = str[i] - '0';

	card->points = str[i] - '0';
	// SDL_Log("Generate %s (%d) | Src (%d,%d) Cost: %d %d %d %d",
	// 	card->id, card->_id,
	// 	card->sprite._src.x, card->sprite._src.y,
	// 	card->cost[0],
	// 	card->cost[1],
	// 	card->cost[2],
	// 	card->cost[3]
	// );
}

int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}


void startTurn(Context *ctx)
{
	ctx->UI.switchMode.enabled = SDL_TRUE;
	for (int i = 0; i < MAX_RESERVE; i++)
		ctx->UI.reserved[i].enabled = SDL_TRUE;
	SDL_Log("Turn started");
}

void endTurn(Context *ctx)
{
	ctx->UI.switchMode.enabled = SDL_FALSE;
	ctx->state = 0;
	for (int i = 0; i < MAX_RESERVE; i++)
		ctx->UI.reserved[i].enabled = SDL_FALSE;
}


void printRect(SDL_Rect *rect, char * msg)
{
	SDL_Log("[%s] Rect (%d,%d) | w: %d h: %d",
			msg,
			rect->x,
			rect->y,
			rect->w,
			rect->h
		);
}


void delReserved(Context *ctx, int id)
{
	Card tmp;
	for (int i = 0; i < ctx->player.reserveCount; i++)
	{
		if (id == ctx->player.reserved[i]._id)
		{
			memcpy(&ctx->player.reserved[i],  &ctx->player.reserved[ctx->player.reserveCount - 1], sizeof(Card));

			ctx->player.reserved[i].sprite.dst = &ctx->player.reserved[i].sprite._dst;
			ctx->player.reserved[i].sprite.src = &ctx->player.reserved[i].sprite._src;

			memset(ctx->player.reserved[ctx->player.reserveCount - 1].id, '0', CARD_ID_LEN);
			memset(ctx->player.reserved[ctx->player.reserveCount - 1].cost, -1, TOK_COUNT - 1);
			ctx->player.reserved[ctx->player.reserveCount - 1]._id	 = -1;

			ctx->player.reserveCount--;
			return ;//return here because if two card have the same id , it will delete both
		}
	}
}

void get_img_src(SDL_Rect *dst, int imageType, int index)
{
	if (index < 0 || index >= CARD_TYPES)
		return ;
	if (imageType == CARD_BACK)
	{
		dst->w = CARD_W;
		dst->h = CARD_H;
		dst->x = OFF_X + (index * (CARD_W + SEP_X));
		dst->y = OFF_Y;
	}
	else if (imageType == CARD)
	{
		dst->w = CARD_W;
		dst->h = CARD_H;
		dst->x = OFF_X + (index * (CARD_W + SEP_X));
		dst->y = OFF_Y + CARD_H + SEP_Y;
	}
	else if (imageType == TOK_HEX)
	{
		dst->w = TOK_W;
		dst->h = TOK_H;
		dst->x = OFF_X + (CARD_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_OFF_X;
		dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
	}
	else if (imageType == TOK_RECT)
	{
		dst->w = TOK_W;
		dst->h = TOK_H;
		dst->x = OFF_X + (CARD_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_W + (TOK_OFF_X * 2);
		dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
	}
}
