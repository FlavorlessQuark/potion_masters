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
	msg[offset++] = ctx->state + '0';
	msg[offset++] = '|';

	offset = compose_player_state(&ctx->players[player], ctx->turn == player, offset);
	offset = compose_board_state(&ctx->board, offset);
	msg[offset] = '\0';
	SDL_Log("Sending message to player %d : %s len (%d)",
			player, msg, offset);
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
	// msg[offset++] = player->actionsRemaining;
	msg[offset++] = 2 + '0';
	msg[offset++] = '|';
	msg[offset++] = '[';
	for (int i = 0; i < ESSENCE_TYPES; i++)
	{
		msg[offset++] = player->tokens[i] + '0';
		msg[offset++] = ',';
	}
	msg[offset++] = ']';
	msg[offset++] = player->potionCount + '0';
	msg[offset++] = '[';
	for (int i = 0; i < player->potionCount; i++)
	{
		SDL_memcpy(msg + offset, player->owned[i].id, player->owned[i].id[0] - '0');
		offset += player->owned[i].id[0] - '0';
		msg[offset++] = ',';
	}
	msg[offset++] = ']';
	msg[offset++] = '|';
	msg[offset++] = player->isBrewing + '0';
	msg[offset++] = ':';
	if (player->isBrewing)
	{
		SDL_memcpy(msg + offset, player->brewing.id, player->brewing.id[0] - '0');
		offset += player->brewing.id[0] - '0';
	}
	msg[offset++] = '|';

	return offset;
}

int compose_board_state(Board *board, int offset)
{
	msg[offset++] = board->masterCount + '0';
	msg[offset++] = '[';
	for (int i = 0; i < ROW_COUNT; i++)
	{
		msg[offset++] = board->rows->recipeCount + '0';
		msg[offset++] = ':';
		for (int j = 0; j < board->rows[i].recipeCount; j++)
		{
			msg[offset++] = '<';
			SDL_memcpy(msg + offset, board->rows[i].recipes[j].id, board->rows[i].recipes[j].id[0] - '0');
			offset += board->rows[i].recipes[j].id[0] - '0';
			msg[offset++] = '>';
			msg[offset++] = ',';
		}
		msg[offset++] = '|';
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

	playr = &ctx->players[ctx->turn];
	potion_idx = msg[1];
	if (msg[0] == 'e')
		ctx->turn = (ctx->turn + 1) % ctx->playerCount;
	else if (msg[0] == 'u')
	{
		playr->owned[potion_idx].fill -= 1;

		if (potions_by_id[playr->owned[potion_idx].type].fn)
			potions_by_id[playr->owned[potion_idx].type].fn(ctx);
		else
			SDL_Log("This really shouldn't happen (this potiont cant be used %d)", playr->owned[potion_idx].type);
		// set fill -1 or del
		// lookup potion from table and do function
	}
	else if (msg[0] == 'b')
	{
		uint8_t rows;
		uint8_t col;

		rows = msg[1] / 3;
		col = msg[1] / MAX_ROWCARD;
		playr->isBrewing = SDL_TRUE;
		playr->brewing = ctx->board.rows[rows].recipes[col];
		// Get potion from board
		// Set "brewing" potion
		// generate new potion
		SDL_memcpy(playr->brewing.id , ctx->board.rows[rows].recipes[col].id, CARD_ID_LEN);
		generatePotion(ctx, &ctx->board.rows[rows].recipes[col], rows);
	}
	else if (msg[0] == 'r') {
		playr->owned[potion_idx].fill -= 1;

		for (int i = 0; i < ESSENCE_TYPES; ++i)
		{
			playr->tokens[i] += potions_by_id[playr->owned[potion_idx].type].essences[i];
		}
		// set fill -1 or delete potion
		// add essences to player
	}
	playr->actionsRemaining -= 1;
}
