#include "../includes/splendor.h"
#define NUMS "0123456789"


SDLX_RectContainer *parseUI(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}

void fillCard(Card *card, int _id, char *id)
{
	int type;

	type = id[1] - '0';
	memcpy(card->id, id, CARD_ID_LEN - 1);
	card->_id = _id;
	if (_id <= 0)
		return ;
	get_img_src(&card->sprite._src, CARD, type);
	for (int i = 0; i < CARD_TYPES; i++)
		card->cost[i] = id[3 + i] - '0';


	SDL_Log("Generate %s (%d) | Src (%d,%d) Cost: %d %d %d %d",
		card->id, _id,
		card->sprite._src.x, card->sprite._src.y,
		card->cost[0],
		card->cost[1],
		card->cost[2],
		card->cost[3]
	);
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
		SDL_Log("COMPARING %d %d", id, ctx->player.reserved[i]._id );
		if (id == ctx->player.reserved[i]._id)
		{
			SDL_Log("FOUND");
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
