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

int parse_player_state(int offset, char *inc_msg);
int parse_board_state(int offset, char * inc_msg);

void send_action(char action, Potion *potion)
{
	msg[0] = action;
	msg[1] = ':';
	if (potion)
	{
		memcpy(msg + 2, potion->id, potion->id[0] - '0');
	}
	else
		msg[1] = '\0';
	sendMessage(msg);
}

void parse_message(Context *ctx, char *inc_msg)
{
	int offset;

	offset = 0;

	if (ctx->state == CONNECT)
	{
		SDL_Log("Parsin msg %s", inc_msg);
		if (inc_msg[0] == 'c')
		{
			char name[10] = {"Player 0"};
			ctx->player.id = inc_msg[1] - '0' + 1;
			ctx->connection.status = CONNECTED;
			name[7] = inc_msg[1] + 1;
			overlay_text(ctx->connection.name.texture, NULL, NULL, 0x000000FF, name);
		}
		else if (inc_msg[0] == 'r')
		{
			SDL_Rect src = {.x = 0, .y = 470, .w = 310, .h = 180};
			overlay_text(ctx->connection.connectSprite.texture, ctx->assets.texUI, &src,0x000000FF, "Unready");
			ctx->connection.status = READY;
		}
		else if (inc_msg[0] == 'u')
		{
			SDL_Rect src = {.x = 0, .y = 472, .w = 310, .h = 180};
			overlay_text(ctx->connection.connectSprite.texture, ctx->assets.texUI, &src, 0x000000FF, "Ready");
			ctx->connection.status = CONNECTED;
		}
	}
	else
	{
		SDL_Log("Stage %d", inc_msg[offset++] - '0');
		offset++;

		offset = parse_player_state(offset, inc_msg);
		SDL_Log("Board state starts here %s", inc_msg + offset);
		offset = parse_board_state(offset, inc_msg);

		SDL_Log("Parsed len %d", offset);
	}
}

int parse_player_state(int offset, char *inc_msg)
{
	SDL_Log("Player status %d", inc_msg[offset++] - '0');
	offset += 1;
	SDL_Log("Player actions %d", inc_msg[offset++] - '0');

	offset += 2;
	for (int i = 0; i < ESSENCE_TYPES; i ++)
	{
		SDL_Log("Player essence %d count %d", i, inc_msg[offset++] - '0');
		offset++;
	}
	offset++;
	int count = inc_msg[offset++] - '0';
	offset++;
	char ID[CARD_ID_LEN];
	SDL_Log("Player potions %d",count);
	// return 0;
	for (int i = 0; i < count; i++)
	{
		SDL_memcpy(ID, inc_msg + offset, inc_msg[offset] - '0');
		SDL_Log("Player potions %d  %s",i, ID);
		offset +=  inc_msg[offset] - '0';
		offset++;
	}
	offset += 2;
	int isBrewing = inc_msg[offset] - '0';
	offset+=2;

	if (isBrewing)
	{
		SDL_memcpy(ID, inc_msg + offset,  inc_msg[offset] - '0');
		SDL_Log("Player potions brew ID %s", ID);
		offset +=  inc_msg[offset] - '0';
	}
	offset++;
	return offset;
}

int parse_board_state(int offset, char *inc_msg)
{
	char ID[CARD_ID_LEN];
	int masterCount =  inc_msg[offset] - '0';
	SDL_Log("Board : master count %d ", masterCount);
	offset+=2;
	for (int i = 0; i < ROW_COUNT; i++)
	{

	}

	for (int i = 0; i < ROW_COUNT; i++)
	{
		int count = inc_msg[offset] - '0';
		SDL_Log("Row %d count %d",i,  count);
		offset+=2;
		for (int j = 0; j < count; j++)
		{
			offset++;
			// SDL_Log("ID starts here %s", inc_msg + offset);
			SDL_memcpy(ID, inc_msg + offset,  inc_msg[offset] - '0');
			offset +=  inc_msg[offset] - '0';
			offset += 2;
			SDL_Log("ID %d %s", j, ID);
		}
		offset++;
	}
	return offset;
}
