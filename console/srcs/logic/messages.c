#include "../includes/splendor.h"

#define PLAYER_STATE_LEN (sizeof(char)+ \
						 sizeof(char) + \
						(sizeof(char) * 2 * ESSENCE_TYPES) + \
						(sizeof(char) * 2 + (MAX_POTIONS)  * CARD_ID_LEN) + \
						(sizeof(char) * 2 + CARD_ID_LEN)   + \
						 sizeof(char) * 2 + \
						 sizeof(char) * 2)

#define BOARD_STATE_LEN (sizeof(char) * 2 + (MAX_MASTER_POTIONS) * CARD_ID_LEN) + \
						(ROW_COUNT * (sizeof(char) * 2 + (MAX_POTIONS * CARD_ID_LEN)))



#define MSG_LEN PLAYER_STATE_LEN + BOARD_STATE_LEN


static char *msg[MSG_LEN];
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
	Rows: <count>:<ids>

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
Brew:    b:<potion_id>
Recycle: r:<potion_id>
End :    e:0

*/

int send_game_state(Context *ctx, int player)
{
	int offset;

	offset = 0;
	msg[offset] = ctx->state + '0';
}