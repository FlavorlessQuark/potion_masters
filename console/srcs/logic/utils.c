#include "../includes/splendor.h"

#include "../includes/table.h"


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
	 Potions ids: <type>|<current_fill>|<cost_count>:<cost>
	Type:          integer
	Current_fill : integer
	Cost: [<identifier><type>:<count>,pas aux ]
		identifier:p or e
		type:      integer
 */

int generatePotion(Context *ctx, Potion *card, int level)
{
	int type;
	int count;
	int offset;
	int i;

	offset = 0;
	type = (rand() % potions_by_tier[level].count) + potions_by_tier[level].entries[0].id;
	card->type = type;

	SDL_memset(card->id, '0', CARD_ID_LEN);
	SDL_itoa(type, card->id, 10);
	offset++;
	if (type >= 10)
		offset++;

	card->id[offset++] = '|';
	card->id[offset++] = MAX_FILL + '0';
	card->id[offset++] = '|';

	count = (rand() % (level + 1) * 2) + level;
	SDL_itoa(potions_by_id[type].cost_count, card->id + offset, 10);
	offset++;
	if (potions_by_id[type].cost_count >= 10)
		offset++;
	card->id[offset] = '|';
	for ( i = 0; i < potions_by_id[type].cost_count; i++)
	{
		offset++;
		SDL_itoa(potions_by_id[type].potion_cost[i], card->id + offset, 10);
		offset++;
		if (potions_by_id[type].potion_cost[i] >= 10)
			offset++;
		card->id[offset] = '-';
	}
	card->id[offset++] = '|';
	i = 0;
	uint8_t essences[ESSENCE_TYPES] = {0};
	while (i < count)
	{
		int amount;
		int tok;

		tok = rand() % ESSENCE_TYPES;
		amount = rand() % MAX(1, count - i) + 1;
		essences[tok] += amount;
		i += (amount + 1);
	}

	for (int i = 0; i < ESSENCE_TYPES; ++i)
	{
		// SDL_itoa(essences[i], card->id + offset, 10);
		SDL_itoa(potions_by_id[type].essences[i], card->id + offset, 10);
		offset++;
		card->id[offset++] = ',';

	}
	card->id[offset] = '\0';

	SDL_Log("Generate %c  %s %d vs %d",
		card->id[0],card->id, strlen(card->id), CARD_ID_LEN
	);

	overlay_text(card->sprite.texture, NULL, NULL, 0xFFFFFFFF, potions_by_id[type].name);

		SDL_Renderer *renderer;
		SDL_Texture *renderTarget;
		SDL_Rect bounds = {0};
		uint32_t color;
		char cost[2] = {"00"};

		renderer = SDLX_DisplayGet()->renderer;
		renderTarget = SDL_GetRenderTarget(renderer);

		SDL_SetRenderTarget(renderer, card->sprite.texture);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		// SDL_RenderClear(renderer);
		SDL_GetRendererOutputSize(renderer, &bounds.w, &bounds.h);

		bounds.y = bounds.h - (bounds.h / 5);
		bounds.w /= 5;
		bounds.h /= 5;
		for (int i = 0; i < ESSENCE_TYPES; ++i)
		{
			color = ((0xFF000000 >> (5 * i)) + 0xFF);
			SDL_itoa(potions_by_id[type].essences[i], cost, 10);
			SDLX_RenderMessage(SDLX_DisplayGet(), &bounds, (SDL_Color){
													.r = (color & ((uint32_t)(0xFF << 24))) >> 24,
													.g = (color & ((uint32_t)(0xFF << 16))) >> 16,
													.b = (color & ((uint32_t)(0xFF << 8))) >> 8,
													.a = (color & ((uint32_t)(0xFF << 0))) >> 0}
													, cost);
			bounds.x += bounds.w;

		}
		SDL_SetRenderTarget(renderer, renderTarget);

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
