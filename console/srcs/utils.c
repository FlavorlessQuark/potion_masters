#include "../includes/splendor.h"

Card *findCard(Context *ctx, char *id, int _id)
{
	int level;
	int i;

	i = 0;
	level = id[0] - '0';
	SDL_Log("Finding %s (%d)", id, _id);
	for (i = 0; i < MAX_ROWCARD; i++)
	{
		SDL_Log("Row %d n %d (%d) <-> (%d)", level, i, ctx->board.rows[level].revealed[i]._id , _id);
		if (ctx->board.rows[level].revealed[i]._id == _id)
			return &ctx->board.rows[level].revealed[i];
	}
	return NULL;
}


int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}


void delReserved(Player *player, int cardId)
{
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		if (player->reserved[i]._id == cardId)
		{
			player->reserved[i] = player->reserved[player->reserveCount - 1];
			player->reserveCount--;
			break ;
		}
	}
}



int generateCard(Card *card, int level)
{
	int type;
	int variation;
	int arr[TOK_COUNT - 1] = {0,1,2,3};
	int singleMax = 3 + (2 * level);
	int totalMax = 6 + (2 * level);
	int length = TOK_COUNT - 1;
	int cost;
	int i;

	type = rand() % CARD_TYPES;
	variation = 0;
	card->sprite._src.y = CARD_OFF_Y;
	card->sprite._src.x = SEP_X + (CARD_W + SEP_X) * type;

	while (totalMax > 0 && length > 0)
	{
		i = rand() % length;
		cost = rand() % MIN(singleMax, totalMax);
		totalMax -= cost;
		card->cost[arr[i]] = cost;
		arr[i] = arr[length - 1];
		length -= 1;
	}


	card->id[0] = level + '0';
	card->id[1] = type + '0';
	card->id[2] = variation + '0';
	card->id[3] =  card->cost[TOK_A] + '0';
	card->id[4] =  card->cost[TOK_B] + '0';
	card->id[5] =  card->cost[TOK_C] + '0';
	card->id[6] =  card->cost[TOK_D] + '0';
	card->id[7] = '\0';
	extract_num(card->id, &card->_id);
	SDL_Log("Generate %s  (%d)| Src (%d,%d) Cost: %d %d %d %d",
		card->id,
		card->_id,
		card->sprite._src.x, card->sprite._src.y,
		card->cost[0],
		card->cost[1],
		card->cost[2],
		card->cost[3]
	);

	return 1;
}

void cleanupUIConfig(SDLX_RectContainer *container)
{
	for (int i = 0; i < container->containerCount; i++)
		cleanupUIConfig(&container->containers[i]);
	if (container->containerCount > 0)
		SDL_free(container->containers);
	if (container->elemCount > 0)
		SDL_free(container->elems);
}

void cleanup(Context *ctx)
{
	SDL_free(ctx);
}
