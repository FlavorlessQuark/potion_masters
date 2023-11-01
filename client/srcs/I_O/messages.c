#include "../includes/splendor.h"


static char msg[MSG_LEN];

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

 Potions ids: <type>|<current_fill>: <max_fill>|<cost>
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

Use:     u:<potion_id>
Purchase:p:<potion_id>
Brew:    b:<potion_id>
Recycle: r:<potion_id>
End :    e

*/

#define NUMS "0123456789"

static int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}

int parse_player_state(Context *ctx, int offset, char *inc_msg);
int parse_board_state(Context *ctx, int offset, char * inc_msg);

void send_action(char action, int position)
{
	msg[0] = action;
	msg[2] = '\0';
	msg[3] = '\0';
	SDL_itoa(position, msg + 1, 10);

	sendMessage(msg);
}

void parse_message(Context *ctx, char *inc_msg)
{
	int offset;

	offset = 0;


		SDL_Log("Parsin msg %s", inc_msg);
		ctx->state = CONNECT;
		if (inc_msg[0] == 'c')
		{
			char name[10] = {"Player 0"};
			ctx->player.id = inc_msg[1] - '0' + 1;
			ctx->connection.status = CONNECTED;
			name[7] = inc_msg[1] + 1;
			overlay_text(ctx->connection.name.texture, NULL, NULL, BLACK, 0.5, name);
		}
		else if (inc_msg[0] == 'r')
		{
			SDL_Rect src = {.x = 0, .y = 470, .w = 310, .h = 180};
			overlay_text(ctx->connection.connectSprite.texture, ctx->assets.texUI, &src, BLACK, 0.5, "Unready");
			ctx->connection.status = READY;
		}
		else if (inc_msg[0] == 'u')
		{
			SDL_Rect src = {.x = 0, .y = 472, .w = 310, .h = 180};
			overlay_text(ctx->connection.connectSprite.texture, ctx->assets.texUI, &src, BLACK, 0.6, "Ready");
			ctx->connection.status = CONNECTED;
		}
		else if (inc_msg[0] == 's')
		{
			if (ctx->state == CONNECT)
			{
				ctx->state = PLAYERSTATUS;
				set_main_cards_active(ctx, SDL_TRUE);
			}
			offset = parse_player_state(ctx, offset + 2, inc_msg);
			offset = parse_board_state(ctx, offset, inc_msg);
		}
}


int parse_player_state(Context *ctx,int offset, char *inc_msg)
{
	char *text = {"Actions  "};
	ctx->isTurn = inc_msg[offset++] - '0';
	offset += 1;
	text[8] = inc_msg[offset];
	ctx->player.actionsRemaining = inc_msg[offset++] + '0';
	SDL_Log("wut %s", text);
	overlay_text(ctx->mainUI.actions.texture, NULL, NULL, 0xFFFFFFFF, 0.8, text);
	offset += 2;
	char count[2] = {"00"};
	SDL_Rect src = {.x = 20, .y = 0, .w = 260, .h = 360};
	for (int i = 0; i < ESSENCE_TYPES; i ++)
	{
		offset += extract_num(inc_msg + offset, &ctx->player.tokens[i]) + 1;
		SDL_itoa(ctx->player.tokens[i], count, 10);
		overlay_text(ctx->mainUI.essences[i].texture, ctx->assets.essence, &src, 0x000000FF, 0.8, count);

		if (i == 1)
		{
			src.x = 20;
			src.y += src.h + 10;
		}
		else
			src.x += src.w;
	}
	offset++;
	ctx->player.ownedCount = inc_msg[offset++] - '0';
	offset++;
	for (int i = 0; i < ctx->player.ownedCount; i++)
	{
		SDL_memcpy(ctx->player.owned[i].id, inc_msg + offset, CARD_ID_LEN);
		generatePotion(ctx, &ctx->player.owned[i]);
		SDL_Log("Player potions %d  %s fill: %d",i, ctx->player.owned[i].id, ctx->player.owned[i].fill);
		offset += CARD_ID_LEN;
		offset++;
	}
	offset += 2;
	ctx->player.isBrewing = inc_msg[offset] - '0';
	offset+=2;
	if (ctx->player.isBrewing)
	{
		SDL_memcpy(ctx->player.brewing.id, inc_msg + offset, CARD_ID_LEN);
		generatePotion(ctx, &ctx->player.brewing);
		SDL_Log("Player potions brew ID %s", ctx->player.brewing.id);
		overlay_text(ctx->player.brewing.sprite.texture, NULL, NULL, WHITE, 1, "Brewing...");
		offset += CARD_ID_LEN;
	}
	offset++;
	return offset;
}

int parse_board_state(Context *ctx,int offset, char *inc_msg)
{
	char ID[CARD_ID_LEN];
	int masterCount =  inc_msg[offset] - '0';
	SDL_Log("Board : master count %d ", masterCount);
	offset+=2;

	for (int i = 0; i < ROW_COUNT; i++)
	{
		ctx->board.rows[i].count = inc_msg[offset] - '0';
		offset+=2;
		for (int j = 0; j < ctx->board.rows[i].count; j++)
		{
			offset++;
			SDL_memcpy(ctx->board.rows[i].card[j].id, inc_msg + offset, CARD_ID_LEN);
			generatePotion(ctx, &ctx->board.rows[i].card[j]);
			offset +=  CARD_ID_LEN;
			offset += 2;
			SDL_Log("ID %d %s", j, ctx->board.rows[i].card[j].id);
		}
	}
	return offset;
}
