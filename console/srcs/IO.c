#include "../includes/splendor.h"

char boardState[TOK_COUNT + (ROW_COUNT * (MAX_ROWCARD * 2) * 2) + 2];

// b[tok1, tok2, tok3, tok4, tok5]|[r0c0 ID] ||[r0c1 ID]| |[r0c2 ID] ...
int sendBoardState(Context *ctx, int player)
{
	int offset;

	offset = 0;
	boardState[0] = 'b';
	boardState[TOK_COUNT + (ROW_COUNT * (MAX_ROWCARD * 2) * 2) + 2 - 1] = '\0';
	for (int i = 0; i < TOK_COUNT; i++)
		boardState[++offset] = ctx->board.tokens[i] + '0';
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int x = 0; x < MAX_ROWCARD; x++)
		{
			if (boardState[++offset] = (ctx->board.rows[i].revealed[x] != NULL) + '0')
			{
				char cardId[4];
				SDL_itoa(ctx->board.rows[i].revealed[x]->id, cardId, 10);
				for (int s = 0; cardId[s] != '\0'; s++)
					boardState[++offset] = cardId[s];
			}
			else
				boardState[++offset] = '0';
			boardState[++offset] = '|';
		}
	}
	boardState[offset] = '\0';
	SDL_Log("Send State %s  to %d", boardState, player);
	send_to(ctx->players[player].handle, boardState);
}

// msg = "Player-Action"
// Action -> Reserve : r[card-id]|
// 		  -> Pay :  p[isReserveCard][card_id]|[tok1, tok2, tok3, tok4, tok5] row is always 0 if reserved card
//        -> Take : t[tok1, tok2, tok3, tok4]
//Example : 1r12 , 2p042|10201, 4t00111, 2p142|10201


int execMsg(Context *ctx, char *msg)
{
	int playerID;
	int cardId;
	int amount;
	uint8_t row;
	uint8_t col;
	int isReserved;

	if (msg == NULL)
		return 0;
	SDL_Log("msg %s", msg);
	playerID = msg[0] - '0';
	msg++; // Assume player is a 1 digit number (as it should); TODO: Check that this is true
	if (msg[0] == 'r')
	{
		msg++;
		msg += extract_num(msg, &cardId);
		findCard(ctx, cardId, &row, &col);
		SDL_Log("Player %d , reserve card %d | r %d c %d", playerID, cardId, row, col);
		ctx->players[playerID].tokens[TOK_R]++;
		ctx->players[playerID].reserved[ctx->players[playerID].reserveCount] = ctx->board.rows[row].revealed[col];
		replaceCard(&ctx->board.rows[row], col);
		if(ctx->players[playerID].reserveCount < MAX_RESERVE)
			ctx->players[playerID].reserveCount++;
		SDL_Log("Player %d ,reserve %d", ctx->players[playerID].reserveCount);
		return 1;
	}
	else if (msg[0] == 'p')
	{
		msg++;
		isReserved = msg[0] - '0';
		msg++;
		msg += extract_num(msg, &cardId);
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
		{
			payReserved(&ctx->players[playerID], cardId);
		}
		else
		{
			findCard(ctx, cardId, &row, &col);
			replaceCard(&ctx->board.rows[row], col);
		}
		return 1;
	}
	else if (msg[0] == 't') { // TODO: Fix redudant if statement
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
		return 1;
	}
	return 0;
}
