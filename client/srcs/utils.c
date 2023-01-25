#include "../includes/splendor.h"

#define NUMS "0123456789"

char *createMsg(Context *ctx)
{
	SDLX_Input input;
	static char *msg;
	static int count;

	if (msg == NULL)
	{
		msg = calloc(20, sizeof(char));
	}
	input = SDLX_InputGet();
	SDL_memset(msg, 0, 20);

	count++;
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		// SDL_Log("Card %d %d %d", i, (ctx->board.rows[0].cardButton[i].triggered), input.mouse_buttons[SDL_BUTTON_LEFT]);
		// if ((ctx->board.rows[0].cardButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		// {
		// 	msg[0] = '0';
		// 	msg[1] = 'p';
		// 	msg[2] = '0';
		// 	msg[3] = '0';
		// 	msg[4] = i + '0';
		// 	msg[5] = '0';
		// 	msg[6] = ctx->board.rows[0].revealed[i]->cost[0] + '0';
		// 	msg[7] = ctx->board.rows[0].revealed[i]->cost[1] + '0';
		// 	msg[8] = ctx->board.rows[0].revealed[i]->cost[2] + '0';
		// 	msg[9] = ctx->board.rows[0].revealed[i]->cost[3] + '0';
		// 	msg[10] = ctx->board.rows[0].revealed[i]->cost[4] + '0';
		// 	msg[11] = '/0';
		// 	SDL_Log("Buy card %d", count);
		// 	SDL_Log("MEssage %s", msg);
		// }
		// else if ((ctx->board.rows[0].cardButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		if ((ctx->board.rows[0].cardButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		{
			msg[0] = '0';
			msg[1] = 'r';
			msg[2] = '0';
			msg[3] = '|';
			msg[4] = '0';
			msg[5] = i + '0';
			msg[6] = '\0';
			SDL_Log("MEssage %s", msg);
			return msg;
		}
	}
	for (int i = 0; i <ctx->player.reserveCount ; i++)
	{
		if ((ctx->player.reservedButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		{
			msg[0] = '0';
			msg[1] = 'p';
			msg[2] = '1';
			msg[3] = '0';
			msg[4] = i + '0';
			msg[5] = '0';
			msg[6] = '|';
			msg[7] = ctx->player.reserved[i]->cost[0] + '0';
			msg[8] = ctx->player.reserved[i]->cost[1] + '0';
			msg[9] = ctx->player.reserved[i]->cost[2] + '0';
			msg[10] = ctx->player.reserved[i]->cost[3] + '0';
			msg[11] = '0';
			msg[12] = '\0';
			SDL_Log("Buy reserve %s", msg);
			return msg;
		}
	}

	// SDL_Log("MEssage %s", msg);
	return NULL;
}

// msg = "Player-Action"
// Action -> Reserve : r[card-id]|[row][col]
// 		  -> Pay : p|[isReserveCard]|[row][col]|[card_id]|[tok1, tok2, tok3, tok4, tok5] row is always 0 if reserved card
//        -> Take : t[tok1, tok2, tok3, tok4]
//Example : 1r12 , 2p01342|10201, 4t00111, 2p10042|10201


void parseMsg(Context *ctx, char *msg)
{
	if (msg[0] == 'c')
	{
		ctx->player.id = msg[1] -'0';
		SDL_Log("Connected as Player %d", ctx->player.id);
	}
	if (msg[0] == 's')
	{
		ctx->state = PLAYERSTATUS;
		SDL_Log("Game starting");
	}
}
