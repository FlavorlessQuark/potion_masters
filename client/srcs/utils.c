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
	card->sprite._src.y = CARD_OFF_Y;
	card->sprite._src.x = SEP_X + (CARD_W + SEP_X) * type;
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
			memcpy(ctx->player.reserved[i].id, ctx->player.reserved[ctx->player.reserveCount - 1].id, CARD_ID_LEN);
			memcpy(ctx->player.reserved[i].cost, ctx->player.reserved[ctx->player.reserveCount - 1].cost, TOK_COUNT - 1);
			memcpy(&tmp, &ctx->player.reserved[i], sizeof(Card));

			ctx->player.reserved[i]._id = ctx->player.reserved[ctx->player.reserveCount - 1]._id;
			ctx->player.reserved[i].points = ctx->player.reserved[ctx->player.reserveCount - 1].points;
			ctx->player.reserved[i].type = ctx->player.reserved[ctx->player.reserveCount - 1].type;
			ctx->player.reserved[i].sprite._dst = ctx->player.reserved[ctx->player.reserveCount - 1].sprite._dst;
			ctx->player.reserved[i].sprite._src = ctx->player.reserved[ctx->player.reserveCount - 1].sprite._src;

			memset(ctx->player.reserved[ctx->player.reserveCount - 1].id, '0', CARD_ID_LEN);
			memset(ctx->player.reserved[ctx->player.reserveCount - 1].cost, -1, TOK_COUNT - 1);
			ctx->player.reserved[ctx->player.reserveCount - 1]._id	 = -1;
			ctx->player.reserved[ctx->player.reserveCount - 1].sprite._dst = tmp.sprite._dst;
			ctx->player.reserved[ctx->player.reserveCount - 1].sprite._src = tmp.sprite._src;

			ctx->player.reserveCount--;
			return ;//return here because if two card have the same id , it will delete both
		}
	}
}
