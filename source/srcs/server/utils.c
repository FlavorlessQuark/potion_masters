#include "../includes/game.h"
#include "../includes/table.h"


#define POTION_PER_ROW (5)
#define HORZ_SEP (10)
#define VERT_SEP (10)


void start_next_turn(Context *ctx)
{
	ctx->players[ctx->turn].actionsRemaining = 0;
	ctx->turn = (ctx->turn + 1)  % ctx->playerCount;
	ctx->players[ctx->turn].actionsRemaining = MAX_ACTIONS;
	if (ctx->players[ctx->turn].isBrewing)
	{
		int count = ctx->players[ctx->turn].potionCount;
		ctx->players[ctx->turn].isBrewing = SDL_FALSE;
		copy_potion(&ctx->players[ctx->turn].owned[count], &ctx->players[ctx->turn].brewing);

		overlay_text(ctx->players[ctx->turn].owned[count].sprite.texture, NULL, NULL, 0xFFFFFFFF,
			potions_by_id[ctx->players[ctx->turn].owned[count].type].name
		);
		ctx->players[ctx->turn].owned[count].sprite._src.w = 370;
		ctx->players[ctx->turn].owned[count].sprite._src.h = 540;
		ctx->players[ctx->turn].owned[count].sprite._src.y = (ctx->players[ctx->turn].owned[count].type / POTION_PER_ROW) * (ctx->players[ctx->turn].owned[count].sprite._src.h + VERT_SEP);
		ctx->players[ctx->turn].owned[count].sprite._src.x = (ctx->players[ctx->turn].owned[count].type % POTION_PER_ROW) * (ctx->players[ctx->turn].owned[count].sprite._src.w + HORZ_SEP);
		ctx->players[ctx->turn].potionCount++;
	}
	for (int i = 0; i < ctx->board.masterCount; ++i)
	{
		if (can_buy_potion(&ctx->players[ctx->turn] ,&ctx->board.master[i]))
		{
			ctx->winner = ctx->turn;
			ctx->state = WIN;
			// SDL_Log("PLAYER %d WINS", ctx->turn);
			char msg[2] = {"w0"};
			msg[1] = ctx->turn + '0';
			for (int i = 0; i < ctx->playerCount; ++i)
			{
				send_to(ctx->players[i].handle, msg);
			}
			return ;
		}
	}
	for (int i = 0; i < ctx->playerCount; ++i)
	{
		send_game_state(ctx, i);
	}
}

void startGame(Context *ctx)
{
	ctx->turn = -1;
	init_new_game(ctx);
	start_next_turn(ctx);
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


/*
	 Potions ids: <type>|<current_fill>|<cost_count>:<cost>
	Type:          integer
	Current_fill : integer
	Cost: [<identifier><type>:<count>,pas aux ]
		identifier:p or e
		type:      integer
 */




int generate_potion(Context *ctx, Potion *card, int level)
{
	int type;
	int count;
	int i;
	uint8_t essences[ESSENCE_TYPES] = {0};

	i = 0;
	type = (rand() % (potions_by_tier[level].count - 1)) + potions_by_tier[level].entries[0].id;
	card->type = type;
	card->fill = MAX_FILL;

	SDL_memset(card->cost, 0, sizeof(uint8_t) * ESSENCE_TYPES);
	count = (rand() % ((level + 1) * 4)) + ((level + 1)  * 3);
	if (level != ROW_COUNT)
	{
		while (i < count)
		{
			int amount;
			int tok;

			tok = rand() % ESSENCE_TYPES;
			amount = rand() % MAX(1, count - i) + 1;
			card->cost[tok] += amount;
			i += (amount + 1);
		}
	}
	SDL_snprintf(card->id, CARD_ID_LEN + 1, "%02d-%d-%02d,%02d,%02d,%02d", type, MAX_FILL,
		card->cost[0],
		card->cost[1],
		card->cost[2],
		card->cost[3]
	);

	return 1;
}

void copy_potion(Potion *dst, Potion *src)
{
	dst->fill = src->fill;
	dst->type = src->type;
	SDL_memcpy(dst->id ,src->id, CARD_ID_LEN);
	SDL_memcpy(dst->cost ,src->cost, ESSENCE_TYPES * sizeof(int));

}

int can_buy_potion(Player *player, Potion *potion)
{
	int x;
	for (int i = 0; i < potions_by_id[potion->type].cost_count; ++i)
	{
		for (x = 0; x < player->potionCount; ++x)
		{
			if (player->owned[x].type == potions_by_id[potion->type].potion_cost[i])
				break ;
		}
		if (x == player->potionCount)
			return SDL_FALSE;
	}
	for (int i = 0; i < ESSENCE_TYPES ; ++i)
	{
		if (player->tokens[i] < potion->cost[i])
			return SDL_FALSE;
	}


	return SDL_TRUE;
}
