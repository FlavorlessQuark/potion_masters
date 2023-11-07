#include "../includes/splendor.h"
#include "../includes/table.h"
#include "../includes/debug.h"

#define PLAYER_STATE_LEN (sizeof(char) +                                     \
						  sizeof(char) * 4 +                                 \
						  (sizeof(char) * 2 * ESSENCE_TYPES) +               \
						  (sizeof(char) * 2 + (MAX_POTIONS) * CARD_ID_LEN) + \
						  (sizeof(char) * 2 + CARD_ID_LEN) +                 \
						  sizeof(char) * 2 +                                 \
						  sizeof(char) * 2)

#define BOARD_STATE_LEN (sizeof(char) * 2 + (MAX_MASTER_POTIONS) * CARD_ID_LEN) + \
							(ROW_COUNT * (sizeof(char) * 6 + (MAX_POTIONS * CARD_ID_LEN)))

#define MSG_LEN PLAYER_STATE_LEN + BOARD_STATE_LEN

static char msg[MSG_LEN];

int compose_player_state(Player *player, int status, int offset);
int compose_board_state(Board *board, int offset);

int send_game_state(Context *ctx, int player)
{
	int offset;

	offset = 0;
	msg[offset++] = 's';
	msg[offset++] = '|';

	offset = compose_player_state(&ctx->players[player], ctx->turn == player, offset);
	offset = compose_board_state(&ctx->board, offset);
	msg[offset] = '\0';
	// SDL_Log("Sending message to player %d : %s len (%d)",
			// player, msg, offset);
	send_to(ctx->players[player].handle, msg);
}

/* STATE MESSAGE
<stage>|<player_state>|<board_state>
Stage : 0 - connect , 1 play, 2 over
Player state: <status>|<actions remaining>|<essences>|<potions>|<brewing>|<points>
   Status:   0 or 1 (connected/ready, playing /not playing, won/lost)
   Essences: [essence1Count, essence2Count, ....]
   Potions:  <count>:[potion1_id, potion2_Id, ...]
   Brewing:  <bool>:id
   Points :  integer
Board state : <master_count>:<master_ids>|<rows>
   Rows: <count>:<ids>|

Potions ids: <type>|<current_fill>|<cost_count>:<cost>
   Type:          integer
   Current_fill : integer
   Max_fill :     integer
   Cost: [<identifier>|<type>:<count>]
	   identifier:p or e
	   type:      integer
	   count:     integer

*/

/* PLAYER MESSAGE

<action>:<action_data>

End :    e:0
Use:     u:<potion_id>
Brew:    b:<potion_id>
Recycle: r:<potion_index>
*/

int compose_player_state(Player *player, int status, int offset)
{
	msg[offset++] = status + '0';
	msg[offset++] = '|';
	// msg[offset++] = player->actionsRemaining + '0';
	SDL_itoa(player->actionsRemaining, msg + offset, 10);
	offset++;
	// msg[offset++] = 2 + '0';
	msg[offset++] = '|';
	msg[offset++] = '[';
	for (int i = 0; i < ESSENCE_TYPES; i++)
	{
		// msg[offset++] = player->tokens[i] + '0';
		SDL_itoa(player->tokens[i], msg + offset, 10);
		if (player->tokens[i] >= 10)
			offset++;
		offset++;
		msg[offset++] = ',';
	}
	msg[offset++] = ']';
	msg[offset++] = player->potionCount + '0';
	msg[offset++] = '[';
	for (int i = 0; i < player->potionCount; i++)
	{
		SDL_memcpy(msg + offset, player->owned[i].id, SDL_strlen(player->owned[i].id));
		// SDL_Log("Player has %d card %s",i,   player->owned[i].id);
		offset += SDL_strlen(player->owned[i].id);
		msg[offset++] = ',';
	}
	msg[offset++] = ']';
	msg[offset++] = '|';
	msg[offset++] = player->isBrewing + '0';
	msg[offset++] = ':';
	if (player->isBrewing)
	{
		SDL_memcpy(msg + offset, player->brewing.id, SDL_strlen(player->brewing.id));
		offset += SDL_strlen(player->brewing.id);
	}
	msg[offset++] = '|';

	return offset;
}

int compose_board_state(Board *board, int offset)
{
	msg[offset++] = board->masterCount + '0';
	for (int i = 0; i < board->masterCount; i++)
	{
		msg[offset++] = ':';
		SDL_memcpy(msg + offset, board->master[i].id, SDL_strlen(board->master[i].id));
		offset += SDL_strlen(board->master[i].id);
	}
	msg[offset++] = '|';
	for (int i = 0; i < ROW_COUNT; i++)
	{
		msg[offset++] = board->rows->recipeCount + '0';
		msg[offset++] = ':';
		for (int j = 0; j < board->rows[i].recipeCount; j++)
		{
			msg[offset++] = '<';
			SDL_memcpy(msg + offset, board->rows[i].recipes[j].id, SDL_strlen(board->rows[i].recipes[j].id));
			offset += SDL_strlen(board->rows[i].recipes[j].id);
			msg[offset++] = '>';
			msg[offset++] = ',';
		}
		msg[offset] = '|';
	}
}

int execMsg		(Context *ctx, char *msg)
{
	if (ctx->state == PLAYING)
		parse_action(ctx, msg);
	else
		SDL_Log("Received message");
}


int parse_action(Context *ctx, char * msg)
{
	Player *playr;
	uint8_t potion_idx;
	char count[2] = {"00"};

	playr = &ctx->players[ctx->turn];
	potion_idx = msg[1] - '0';
	if (msg[0] == 'e')
	{
		start_next_turn(ctx);
		return 0 ;
	}
	else if (msg[0] == 'u')
	{
		// SDL_Log("Fill was, %d", playr->owned[potion_idx].fill);
		playr->owned[potion_idx].fill -= 1;
		if (playr->owned[potion_idx].fill > 0)
			playr->owned[potion_idx].id[3] = playr->owned[potion_idx].fill + '0';
		else
		{
			copy_potion(&playr->owned[potion_idx], &playr->owned[playr->potionCount - 1]);
			generate_potion_texture(ctx, &playr->owned[potion_idx]);
			playr->potionCount--;
		}
		// SDL_Log("Fill now, %d", playr->owned[potion_idx].fill);

		if (potions_by_id[playr->owned[potion_idx].type].fn)
			potions_by_id[playr->owned[potion_idx].type].fn(ctx);
		else
			SDL_Log("This really shouldn't happen (this potiont cant be used %d)", playr->owned[potion_idx].type);
	}
	else if (msg[0] == 'b')
	{
		uint8_t rows;
		uint8_t col;
		int n;

		n = SDL_atoi(msg + 1);
		rows = n / MAX_ROWCARD;
		col = n % MAX_ROWCARD;
		// SDL_Log("Buying potion no %d -> %d %d", n, rows, col);
		if (!can_buy_potion(&ctx->players[ctx->turn], &ctx->board.rows[rows].recipes[col]))
		{
			// SDL_Log("Error, can't buy this potion");
			return 0;
		}
		playr->isBrewing = SDL_TRUE;
		for (int i = 0; i < ESSENCE_TYPES; i++)
			playr->tokens[i] -= ctx->board.rows[rows].recipes[col].cost[i];
		draw_player_essences(ctx, playr);
		copy_potion(&playr->brewing, & ctx->board.rows[rows].recipes[col]);
		generate_potion(ctx, &ctx->board.rows[rows].recipes[col], rows);
		overlay_text(playr->brewing.sprite.texture, NULL, NULL, 0xFFFFFFFF, potions_by_id[playr->brewing.type].name);
	}
	else if (msg[0] == 's') {
		playr->owned[potion_idx].fill -= 1;
		if (playr->owned[potion_idx].fill > 0)
			playr->owned[potion_idx].id[3] = playr->owned[potion_idx].fill + '0';
		else
		{
			copy_potion(&playr->owned[potion_idx], &playr->owned[playr->potionCount]);
			generate_potion_texture(ctx, &playr->owned[potion_idx]);
			playr->potionCount--;
		}
		// SDL_Log("Fill now at, %d %s", playr->owned[potion_idx].fill, playr->owned[potion_idx].id);

		for (int i = 0; i < ESSENCE_TYPES; ++i)
			playr->tokens[i] += potions_by_id[playr->owned[potion_idx].type].essences[i];
		draw_player_essences(ctx, playr);
	}
	else
		return 0;
	playr->actionsRemaining -= 1;
}
