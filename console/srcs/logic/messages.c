#include "../includes/splendor.h"

#define PLAYER_STATE_LEN sizeof(char) + \
						 sizeof(char) + \
						(sizeof(char) * 2 * ESSENCE_TYPES)  \
						(sizeof(char) * 2 + (MAX_POTIONS) * CARD_ID_LEN) + \
						(sizeof(char) * 2 + CARD_ID_LEN) + \
						 sizeof(char) * 2 + \
						 sizeof(char) * 2

#define BOARD_STATE_LEN (sizeof(char) * 2 + (MAX_MASTER_POTIONS) * CARD_ID_LEN) + \
						(ROW_COUNT * (sizeof(char) * 2 + (MAX_POTIONS * CARD_ID_LEN)))



#define MSG_LEN PLAYER_STATE_LEN + BOARD_STATE_LEN

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

	// for (int i = 0; i < ESSENCE_TYPES; i++)
	// 	boardState[++offset] = ctx->players[ctx->turn].tokens[i] + '0';
	// boardState[++offset] = ctx->players[ctx->turn].points + '0';
	// for (int i = 0; i < ROW_COUNT; i++)
	// {
	// 	for (int x = 0; x < MAX_ROWCARD; x++)
	// 	{
	// 		// SDL_Log("CARD ID %s", ctx->board.rows[i].revealed[x].id);
	// 		for (int s = 0; ctx->board.rows[i].revealed[x].id[s] != '\0'; s++)
	// 			boardState[++offset] = ctx->board.rows[i].revealed[x].id[s];
	// 		// boardState[++offset] = '|';
	// 	}
	// }
	// // for (int x = 0; x < ctx->players[ctx->turn].reserveCount; x++)
	// // 	{
	// // 		for (int s = 0; ctx->players[ctx->turn].reserved[x].id[s] != '\0'; s++)
	// // 			boardState[++offset] = ctx->players[ctx->turn].reserved[x].id[s];
	// // 		// boardState[++offset] = '|';
	// // 	}
	// boardState[offset + 1] = '\0';
	// SDL_Log("Send State %s  to %d", boardState, player);
	// send_to(ctx->players[player].handle, boardState);
}

// msg = "Player-Action"
// Action -> Reserve : r[card-id]|
// 		  -> Pay :  p[isReservePotion][card_id]|[tok1, tok2, tok3, tok4, tok5] row is always 0 if reserved card
//        -> Take : t[tok1, tok2, tok3, tok4]
// Example : 1r12 , 2p042|10201, 4t00111, 2p142|10201

