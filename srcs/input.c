#include "../includes/splendor.h"

#define NUMS "0123456789"
char *input(Context *ctx)
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
			msg[6] = '/0';
			SDL_Log("MEssage %s", msg);
			return msg;
		}
	}
	for (int i = 0; i <ctx->players[0].reserveCount ; i++)
	{
		if ((ctx->players[0].reservedButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		{
			msg[0] = '0';
			msg[1] = 'p';
			msg[2] = '1';
			msg[3] = '0';
			msg[4] = i + '0';
			msg[5] = '0';
			msg[6] = '|';
			msg[7] = ctx->players[0].reserved[i]->cost[0] + '0';
			msg[8] = ctx->players[0].reserved[i]->cost[1] + '0';
			msg[9] = ctx->players[0].reserved[i]->cost[2] + '0';
			msg[10] = ctx->players[0].reserved[i]->cost[3] + '0';
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
int decodeMsg(Context *ctx, char *msg)
{
	int playerID;
	int cardId;
	int amount;
	int row;
	int col;
	int isReserved;

	if (msg == NULL)
		return 0;
	SDL_Log("msg %s", msg);
	playerID = msg[0] - '0';
	msg++; // Assume player is a 1 digit number (as it should); TODO: Check that this is true
	if (msg[0] == 'r')
	{
		msg++;
		cardId = SDL_atoi(msg);
		msg += 2;
		row = msg[0] - '0';
		msg++;
		col = msg[0] - '0';
		SDL_Log("Player %d , buys reserve card %d | r %d c %d", playerID, cardId, row, col);
		ctx->players[playerID].tokens[TOK_R]++;
		ctx->players[playerID].reserved[ctx->players[0].reserveCount] = ctx->board.rows[row].revealed[col];
		replaceCard(&ctx->board.rows[row], col);
		if(ctx->players[0].reserveCount < MAX_RESERVE)
			ctx->players[0].reserveCount++;
		SDL_Log("Player %d ,reserve %d", ctx->players[0].reserveCount);
	}
	else if (msg[0] == 'p')
	{
		msg++;
		isReserved = msg[0] - '0';
		msg++;
		row = msg[0] - '0';
		msg++;
		col = msg[0] - '0';
		msg++;
		cardId = SDL_atoi(msg);
		msg += strspn(msg,NUMS) + 1;
		SDL_Log("Player %d , Buys card %d from reserve? %d row %d col %d", playerID, cardId, isReserved, row, col);
		for (int i = 0; i < TOK_COUNT; i++)
		{
			amount = msg[0] - '0';
			msg++;
			ctx->players[playerID].tokens[i] -= amount;
			ctx->board.tokens[i] += amount;
			SDL_Log("Token %d: %d", i, amount);
		}
		ctx->players[playerID].owned[ctx->board.rows[row].revealed[col]->type]++;
		if (isReserved)
			payrReserved(&ctx->players[playerID], col);
		else
			replaceCard(&ctx->board.rows[row], col);
	}
	else { // TODO: Fix redudant if statement
		msg++;
		SDL_Log("Player %d , takes", playerID);
		for (int i = 0; i < CARD_TYPES; i++)
		{
			amount = msg[0] - '0';
			SDL_Log("Token %d: %d", i, amount);
			msg++;
			ctx->players[playerID].tokens[i] += amount;
			ctx->board.tokens[i] -= amount;
		}
	}
}
