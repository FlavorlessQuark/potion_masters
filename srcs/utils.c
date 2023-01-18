#include "../includes/splendor.h"



void payrReserved(Player *player, int position)
{
	player->reserved[position] = player->reserved[player->reserveCount - 1];
	player->reserveCount--;
}

int replaceCard(Row *row, int position)
{
	int random;
	Card tmp;

	if (row->remainCount <= 0)
	{
		row->revealed[position] = NULL;
		row->cardButton[position].enabled = SDL_FALSE;
		return 0;
	}


	random = rand() % row->remainCount;
	tmp = row->remaining[row->remainCount - 1];
	row->remaining[row->remainCount - 1] = row->remaining[random];
	row->remaining[random] = tmp;
	row->revealed[position] = &row->remaining[row->remainCount - 1];
	row->revealed[position]->sprite.dst = row->cardButton[position].boundingBox;
	row->remainCount--;

	return 1;
}
