#include "../includes/splendor.h"



// Potion *findPotion(Context *ctx, char *id, int _id)
// {
// 	int level;
// 	int i;

// 	i = 0;
// 	level = id[0] - '0';
// 	SDL_Log("Try find %d", _id);
// 	for (i = 0; i < MAX_ROWCARD; i++)
// 	{
// 		SDL_Log("CMP %s[%d] -> %s[%d]",
// 		ctx->board.rows[level].revealed[i].id, ctx->board.rows[level].revealed[i]._id,
// 		id, _id);
// 		if (ctx->board.rows[level].revealed[i]._id == _id)
// 			return &ctx->board.rows[level].revealed[i];
// 	}
// 	return NULL;
// }

// void nextTurn(Context *ctx)
// {
// 	int i;

// 	i = (ctx->turn + 1) % MAX_PLAYERS;
// 	while (i != ctx->turn)
// 	{
// 		if (ctx->players[i].status == READY)
// 		{
// 			ctx->turn = i;
// 			break ;
// 		}
// 		i = (i + 1) % MAX_PLAYERS;
// 	}
// 	send_game_state(ctx, ctx->turn);
// }

// void startGame(Context *ctx)
// {
// 	ctx->turn = 0;
// 	initNewGame(ctx);
// 	for (int i = 0; i < MAX_PLAYERS; i++)
// 	{
// 		if (ctx->players[i].status == READY)
// 			send_to(ctx->players[i].handle, "s");
// 	}
// 	send_game_state(ctx, ctx->turn);
// }


// int	extract_num(char *str, int *number)
// {
// 	int spn;

// 	spn = strcspn(str, NUMS);
// 	*number = atoi(str + spn);
// 	return spn + strspn(str + spn, NUMS);
// }

// void delReserved(Player *player, int cardId)
// {
// 	// for (int i = 0; i < MAX_RESERVE; i++)
// 	// {
// 	// 	if (player->reserved[i]._id == cardId)
// 	// 	{
// 	// 		player->reserved[i] = player->reserved[player->reserveCount - 1];
// 	// 		player->reserveCount--;
// 	// 		break ;
// 	// 	}
// 	// }
// }



/*
	 Potions ids: <type>|<current_fill>: <max_fill>|<count>|<cost>
	Type:          integer
	Current_fill : integer
	Max_fill :     integer
	Cost: [<identifier>|<type>:<count>]
		identifier:p or e
		type:      integer
		count:     integer
 */

int generatePotion(SDL_Texture *base, Potion *card, int level)
{
	int type;
	int count;
	int i;

	type = rand() % POTION_TYPES;

	card->id[0] = '5';
	card->id[1] = type + '0';
	card->id[2] = ',';
	card->id[3] = 3 + '0';
	card->id[4] = ':';
	card->id[5] = 3 + '0';
	card->id[6] = ',';

	count = (rand() % MAX_COST_TYPES) + 1;

	card->id[7] = count + '0';
	card->id[8] = '|';

	for (int i = 0; i < count; i++)
	{
		card->id[9 + i * 4 + 0] = 'e';
		card->id[9 + i * 4 + 1] = (rand() % POTION_TYPES) + '0';
		card->id[9 + i * 4 + 2] = (rand() % 4) + '0';
		card->id[9 + i * 4 + 3] = ',';
		// SDL_Log("?? %d %c", 8 + i * 4, card->id[8 + i * 4]);
	}
	// SDL_Log("?? %d %c", 8 + i * 4, card->id[8 + i * 3 ]);
	card->id[9 + count * 4] = '\0';
	card->id[0] = strlen(card->id) + '0';


	// extract_num(card->id, &card->_id);
	// generatePotionTexture(base, card, type);
	SDL_Log("Generate %c  %s %d vs %d",
		card->id[0],card->id, strlen(card->id), CARD_ID_LEN
	);
	// SDL_Log("Generate %s  (%d) | Src (%d,%d) Cost: %d %d %d %d, points %d",
	// 	card->id,
	// 	card->_id,
	// 	card->sprite._src.x, card->sprite._src.y,
	// 	card->cost[0],
	// 	card->cost[1],
	// 	card->cost[2],
	// 	card->cost[3],
	// 	card->points
	// );

	return 1;
}




// void cleanup(Context *ctx)
// {
// 	SDL_free(ctx);
// }

void get_img_src(SDL_Rect *dst, int imageType, int index)
{
	// if (index < 0 || index >= POTION_TYPES)
	// 	return ;
	// if (imageType == CARD_BACK)
	// {
	// 	dst->w = CARD_W;
	// 	dst->h = CARD_H;
	// 	dst->x = OFF_X + (index * (CARD_W + SEP_X));
	// 	dst->y = OFF_Y;
	// }
	// else if (imageType == CARD)
	// {
	// 	dst->w = CARD_W;
	// 	dst->h = CARD_H;
	// 	dst->x = OFF_X + (index * (CARD_W + SEP_X));
	// 	dst->y = OFF_Y + CARD_H + SEP_Y;
	// }
	// else if (imageType == TOK_HEX)
	// {
	// 	dst->w = TOK_W;
	// 	dst->h = TOK_H;
	// 	dst->x = OFF_X + (POTION_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_OFF_X;
	// 	dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
	// }
	// else if (imageType == TOK_RECT)
	// {
	// 	dst->w = TOK_W;
	// 	dst->h = TOK_H;
	// 	dst->x = OFF_X + (POTION_TYPES * (CARD_W + SEP_X)) - SEP_X + TOK_W + (TOK_OFF_X * 2);
	// 	dst->y = OFF_Y + (index * (TOK_H + TOK_OFF_Y));
	// }
}
