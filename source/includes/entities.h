# ifndef _GAME_ENTITIES_H
# define _GAME_ENTITIES_H

#include "macros.h"

typedef struct Potion
{
	SDLX_Sprite sprite;
	SDLX_Sprite e_costSprite[ESSENCE_TYPES];
	SDLX_Sprite p_costSprite[3];
	int cost[ESSENCE_TYPES];
	int type;
	int fill;
	int _id;
	char id[CARD_ID_LEN];
}	Potion;

typedef struct Player
{
	Potion owned[MAX_POTIONS];
	Potion brewing;
	int tokens[ESSENCE_TYPES];
	int ownedCount;
	uint8_t id;
	int isBrewing;
	uint8_t points;
	int actionsRemaining;
}	Player;

#endif
