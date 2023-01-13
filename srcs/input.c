#include "../includes/splendor.h"

#define NUMS "0123456789"
char *input(Context *ctx)
{
	SDLX_Input input;
	static char *msg;

	if (msg == NULL)
	{
		msg = calloc(20, sizeof(char));
	}
	input = SDLX_InputGet();
	msg[0] = '0';
	for (int i = 0; i < MAX_ROWCARD; i++)
	{
		if ((ctx->board.rows[0].cardButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		{
			msg[1] = 'p';
			msg[2] = 0;
			msg[3] = 0;
			msg[4] = i + 48;
			msg[5] = 0;
			msg[6] = ctx->board.rows[0].revealed[i]->cost[0];
			msg[7] = ctx->board.rows[0].revealed[i]->cost[1];
			msg[8] = ctx->board.rows[0].revealed[i]->cost[2];
			msg[9] = ctx->board.rows[0].revealed[i]->cost[3];
			SDL_Log("Buy card");
		}
		// if ((ctx->board.rows[0].cardButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_RIGHT] == SDLX_KEYDOWN)
		// {
		// 	SDL_Log("triggered, Reserve %d %d %d %d + %d point| Remaining:  | points : %d");
		// }
	}
	for (int i = 0; i < MAX_RESERVE; i++)
	{
		if ((ctx->players[0].reservedButton[i].triggered) && input.mouse_buttons[SDL_BUTTON_LEFT] == SDLX_KEYDOWN)
		{
			msg[1] = 'p';
			msg[2] = 1;
			msg[3] = 0;
			msg[4] = i + 48;
			msg[5] = 0;
			msg[6] = ctx->players[0].reserved[i]->cost[0];
			msg[7] = ctx->players[0].reserved[i]->cost[1];
			msg[8] = ctx->players[0].reserved[i]->cost[2];
			msg[9] = ctx->players[0].reserved[i]->cost[3];
			SDL_Log("Buy reserve");
		}
	}
	msg[10] = '/0';
	SDL_SetRenderDrawColor(ctx->display->renderer, 0, 0, 0, 255);
	SDL_Log("MEssage %s", msg);
}

// msg = "Player-Action"
// Action -> Reserve : r[card-id]
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

	playerID = SDL_atoi(msg);
	msg++; // Assume player is a 1 digit number (as it should); TODO: Check that this is true
	if (msg[0] == 'r')
	{
		msg++;
		cardId = SDL_atoi(msg);
		ctx->players[playerID].tokens[TOK_R]++;
		ctx->players[playerID].reserved[ctx->players[0].reserveCount] = &ctx->cards[cardId];
		SDL_Log("Player %d , reserve card %d", playerID, cardId);
	}
	else if (msg[0] == 'p')
	{
		msg++;
		isReserved = msg[0] - 48;
		msg++;
		row = SDL_atoi(msg);
		msg++;
		col = SDL_atoi(msg);
		msg++;
		cardId = SDL_atoi(msg);
		msg += strspn(msg,NUMS) + 1;
		SDL_Log("Player %d , Buys card %d from reserve? %d row %d col %d", playerID, cardId, isReserved, row, col);
		for (int i = 0; i < TOK_COUNT; i++)
		{
			amount = SDL_atoi(msg);
			msg += strspn(msg,NUMS);
			ctx->players[playerID].tokens[i] -= amount;
			ctx->board.tokens[i] += amount;
			SDL_Log("Token %d: %d", i, amount);
		}
		if (isReserved)
			payrReserved(&ctx->players[playerID], col);
		else
			replaceCard(&ctx->board.rows[row], col);
		ctx->players[playerID].owned[ctx->cards[cardId].type]++;
	}
	else { // TODO: Fix redudant if statement
		msg++;
		SDL_Log("Player %d , takes", playerID);
		for (int i = 0; i < TOK_COUNT; i++)
		{
			SDL_Log("Token %d: %d", i, amount);
			amount = SDL_atoi(msg);
			msg += strspn(msg,NUMS);
			ctx->players[playerID].tokens[i] += amount;
			ctx->board.tokens[i] -= amount;
		}
	}
}
