#include "../includes/splendor.h"

void findCard(Context *ctx, uint8_t id, uint8_t *row, uint8_t *col)
{
	for (*row = 0; *row < ROW_COUNT;*row += 1)
	{
		for (*col = 0; *col < MAX_ROWCARD; *col += 1)
		{
			if (ctx->board.rows[*row].revealed[*col] != NULL && ctx->board.rows[*row].revealed[*col]->id == id)
				return ;
		}
	}
}


int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}


void payReserved(Player *player, uint8_t cardId)
{
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		if (player->reserved[i]->id == cardId)
		{
			player->reserved[i] = player->reserved[player->reserveCount - 1];
			player->reserveCount--;
			break ;
		}
	}
}

int replaceCard(Row *row, int position)
{
	int random;
	Card tmp;

	if (row->remainCount <= 0)
	{
		row->revealed[position] = NULL;
		return 0;
	}
	random = rand() % row->remainCount;
	tmp = row->remaining[row->remainCount - 1];
	row->remaining[row->remainCount - 1] = row->remaining[random];
	row->remaining[random] = tmp;
	row->revealed[position] = &row->remaining[row->remainCount - 1];
	row->remainCount--;

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
	SDL_free(ctx->cards);

	SDL_free(ctx);
}
