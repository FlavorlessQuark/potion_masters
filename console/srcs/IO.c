#include "../includes/splendor.h"
#define NUMS "0123456789"

int sendBoardState(Context *ctx, int player)
{

}


int execMsg(Context *ctx, char *msg)
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
			payReserved(&ctx->players[playerID], col);
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
