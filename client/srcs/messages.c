#include "../includes/splendor.h"


static char msg[MSG_LEN];

 /* STATE MESSAGE
 <stage>|<player_state>|<board_state>
 Stage : 0 - connect , 1 play, 2 over
 Player state: <status>|<essences>|<potions>|<brewing>|<points>
	Status:   0 or 1 (playing /playing, won/lost)
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
int parse_player_state(int offset, char *inc_msg);
int parse_board_state(int offset, char * inc_msg);

void parse_message(char *inc_msg)
{
	int offset;

	offset = 0;

	SDL_Log("Stage %d", inc_msg[offset++] - '0');
	offset++;

	offset += parse_player_state(offset, inc_msg);
	offset += parse_board_state(offset, inc_msg);

	SDL_Log("Parsed len %d", offset);
}

int parse_player_state(int offset, char *inc_msg)
{
	SDL_Log("Player status %d", inc_msg[offset++] - '0');
	offset++;

	for (int i = 0; i < ESSENCE_TYPES; i ++)
	{
		SDL_Log("Player essence %d count %d", i, inc_msg[offset++] - '0');
		offset++;
	}
	offset++;
	int count = inc_msg[offset++] - '0';
	char ID[CARD_ID_LEN];
	SDL_Log("Player potions %d",count);
	for (int i = 0; i < count; i++)
	{
		SDL_memcpy(ID, inc_msg + offset, CARD_ID_LEN);
		SDL_Log("Player potions %d  %s",i, ID);
		offset += CARD_ID_LEN;
		offset++;
	}
	offset++;
	int isBrewing = inc_msg[offset++] - '0';
	offset++;
	if (isBrewing)
	{
		SDL_memcpy(ID, inc_msg + offset, CARD_ID_LEN);
		SDL_Log("Player potions brew ID %s", ID);
		offset += CARD_ID_LEN;
	}
	offset++;

	return offset;
}

int parse_board_state(int offset, char *inc_msg)
{
	SDL_Log("Board : master count %d ", inc_msg[offset++] - '0');
	offset++;
	char ID[CARD_ID_LEN];
	for (int i = 0; i < ROW_COUNT; i++)
	{
		int count = inc_msg[offset++] - '0';
		SDL_Log("Row %d count %d ",i,  count);
		offset++;
		for (int j = 0; j < count; j++)
		{
			SDL_memcpy(ID, inc_msg + offset, CARD_ID_LEN);
			offset += CARD_ID_LEN;
			offset++;
			SDL_Log("ID %d %s", j, ID);
		}
		offset++;
	}
	return offset;
}
