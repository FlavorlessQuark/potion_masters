#include "../includes/splendor.h"

#define MSG_LEN TOK_COUNT + (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) + 2
char boardState[MSG_LEN];// +2 -> 'b' and '\0'

// b[tok1, tok2, tok3, tok4, tok5]|[r0c0 ID] ||[r0c1 ID]| |[r0c2 ID] ...
int sendBoardState(Context *ctx, int player)
{
	int offset;

	offset = 0;
	boardState[0] = 'b';
	boardState[TOK_COUNT + (ROW_COUNT * CARD_ID_LEN) + 1] = '\0';
	for (int i = 0; i < TOK_COUNT; i++)
		boardState[++offset] = ctx->board.tokens[i] + '0';
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int x = 0; x < MAX_ROWCARD; x++)
		{
			// SDL_Log("CARD -> %d row, no %d -> id %d %p", i, x, ctx->board.rows[i].revealed[x]);
			SDL_Log("CARD ID %s",  ctx->board.rows[i].revealed[x].id);
			for (int s = 0; ctx->board.rows[i].revealed[x].id[s] != '\0'; s++)
			{
				// SDL_Log("Offset %d / %d -> board %s } %d/%d", offset,MSG_LEN, boardState, s,CARD_ID_LEN );
				boardState[++offset] = ctx->board.rows[i].revealed[x].id[s];
			}
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
	int isReserved;
	char *id;
	Card *card;
	SDL_Rect dst;

	if (msg == NULL)
		return 0;
	SDL_Log("msg %s", msg);
	playerID = msg[0] - '0';
	msg++; // Assume player is a 1 digit number (as it should); TODO: Check that this is true
	if (msg[0] == 'r')
	{
		msg++;
		id = msg;
		msg += extract_num(msg, &cardId);
		if (!(card = findCard(ctx, id, cardId)))
			SDL_Log("Couldn't find card %d", cardId);
		SDL_Log("Player %d , reserve card %d", playerID, cardId);
		dst = ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite._dst;
		ctx->players[playerID].tokens[TOK_R]++;
		ctx->players[playerID].reserved[ctx->players[playerID].reserveCount] = *card;
		ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite._dst = dst;
		ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite.dst = &ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite._dst;
		ctx->players[playerID].reserveCount++;
		if (ctx->board.rows[id[0] - '0'].remainCount > 0)
		{
			generateCard(card, id[0] - '0');
			ctx->board.rows[id[0] - '0'].remainCount--;
		}
		SDL_Log("Player %d ,reserve count %d",playerID, ctx->players[playerID].reserveCount);
		return 1;
	}
	else if (msg[0] == 'p')
	{
		msg++;
		id = msg;
		msg += extract_num(msg, &cardId) + 1;
		isReserved = msg[0] - '0';
		msg++;
		// findCard(ctx, cardId, &row, &col);
		SDL_Log("Player %d , Buys card %d from reserve? %d | msg %s", playerID, cardId, isReserved, msg);
		for (int i = 0; i < TOK_COUNT; i++)
		{
			amount = msg[0] - '0';
			msg++;
			ctx->players[playerID].tokens[i] -= amount;
			ctx->board.tokens[i] += amount;
			SDL_Log("Token %d: %d", i, amount);
		}
		ctx->players[playerID].owned[id[1] - '0']++;
		ctx->players[playerID].points += card->points;
		if (isReserved)
		{
			delReserved(&ctx->players[playerID], cardId);
		}
		else
		{
			card = findCard(ctx, id, cardId);
			if (ctx->board.rows[id[0] - '0'].remainCount > 0)
				generateCard(card, id[0] - '0');
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
