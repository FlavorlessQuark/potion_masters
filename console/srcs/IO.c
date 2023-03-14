#include "../includes/splendor.h"

/
#define MSG_LEN \
(TOK_COUNT * 3)  \ /// Board tokens + player permanent & tokens
+ (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) \ // Board card Ids (+1 for separator)
+ (MAX_RESERVE * CARD_ID_LEN + 1) \ // Player Reserved card iDs (+1 for separator)
+  1 \ // Player points
+ 3 \ // Null terminator, type prefix and player ID
char boardState[MSG_LEN];

// b[boardTokens]|[playerTokens][playerPoints][r0c0 ID] ||[r0c1 ID]| |[r0c2 ID] ...|[reservedIDs]
int send_game_state(Context *ctx, int player)
{
	int offset;

	offset = 0;
	boardState[0] = 'b';
	boardState[TOK_COUNT + (ROW_COUNT * CARD_ID_LEN) + 1] = '\0';
	for (int i = 0; i < TOK_COUNT; i++)
		boardState[++offset] = ctx->board.tokens[i] + '0';
	for (int i = 0; i < TOK_COUNT; i++)
		boardState[++offset] = ctx->players[ctx->turn].tokens[i] + '0';
	boardState[++offset] = ctx->players[ctx->turn].points + '0';
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int x = 0; x < MAX_ROWCARD; x++)
		{
			// SDL_Log("CARD ID %s", ctx->board.rows[i].revealed[x].id);
			for (int s = 0; ctx->board.rows[i].revealed[x].id[s] != '\0'; s++)
				boardState[++offset] = ctx->board.rows[i].revealed[x].id[s];
			boardState[++offset] = '|';
		}
	}
	for (int x = 0; x < ctx->players[ctx->turn].reserveCount; x++)
		{
			for (int s = 0; ctx->players[ctx->turn].reserved[x].id[s] != '\0'; s++)
				boardState[++offset] = ctx->players[ctx->turn].reserved[x].id[s];
			boardState[++offset] = '|';
		}
	boardState[offset] = '\0';
	SDL_Log("Send State %s  to %d", boardState, player);
	send_to(ctx->players[player].handle, boardState);
}

// msg = "Player-Action"
// Action -> Reserve : r[card-id]|
// 		  -> Pay :  p[isReserveCard][card_id]|[tok1, tok2, tok3, tok4, tok5] row is always 0 if reserved card
//        -> Take : t[tok1, tok2, tok3, tok4]
// Example : 1r12 , 2p042|10201, 4t00111, 2p142|10201

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
	playerID = msg[0] - '0';
	msg++;

	if (msg[0] == 'r')
		return execReserve(ctx, playerID, msg);
	else if (msg[0] == 'p')
		return execBuy(ctx, playerID, msg);
	else if (msg[0] == 't')
		return execTake(ctx, playerID, msg);

	return 0;
}

int execReserve(Context *ctx, uint8_t playerID, char *msg)
{
	char *id;
	int cardId;
	Card *card;

	msg++;
	id = msg;
	msg += extract_num(msg, &cardId);

	if (!(card = findCard(ctx, id, cardId)))
	{
		SDL_Log("Couldn't find card %d", cardId);
		return 0;
	}
	SDL_Log("Player %d , reserve card %d", playerID, cardId);

	memcpy(ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].cost, card->cost, TOK_COUNT - 1);
	memcpy(ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].id, card->id, TOK_COUNT - 1);

	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount]._id = card->_id;
	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].points = card->points;
	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite.texture = card->sprite.texture;
	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite._src = card->sprite._src;

	ctx->players[playerID].tokens[TOK_R]++;
	ctx->players[playerID].reserveCount++;

	if (ctx->board.rows[id[0] - '0'].remainCount > 0)
	{
		generateCard(ctx->Tcards, card, id[0] - '0');
		ctx->board.rows[id[0] - '0'].remainCount--;
	}
	SDL_Log("Player %d ,reserve count %d", playerID, ctx->players[playerID].reserveCount);
	return 1;
}

int execBuy(Context *ctx, uint8_t playerID, char *msg)
{
	char *id;
	char isReserved;
	int cardId;
	int amount;
	Card *card;

	msg++;
	isReserved = msg[0] - '0';
	msg++;
	id = msg;
	msg += extract_num(msg, &cardId) + 1;

	SDL_Log("Player %d , Buys card %d from reserve? %d | msg %s", playerID, cardId, isReserved, msg);
	for (int i = 0; i < TOK_COUNT; i++)
	{
		amount = msg[0] - '0';
		amount = MAX(amount, ctx->players[playerID].owned[i]);
		if (ctx->players[playerID].tokens[i] < amount)
		{
			ctx->players[playerID].tokens[CARD_TYPES] -= amount - ctx->players[playerID].tokens[i];
			ctx->board.tokens[CARD_TYPES] += amount - ctx->players[playerID].tokens[i];
			amount -= (amount - ctx->players[playerID].tokens[i]);
		}
		ctx->players[playerID].tokens[i] -= amount;
		ctx->board.tokens[i] += amount;
		msg++;
	}
	ctx->players[playerID].owned[id[1] - '0']++;
	ctx->players[playerID].points += id[CARD_ID_LEN - 2] - '0';

	if (isReserved)
		delReserved(&ctx->players[playerID], cardId);
	else
	{
		card = findCard(ctx, id, cardId);
		if (ctx->board.rows[id[0] - '0'].remainCount > 0)
			generateCard(ctx->Tcards, card, id[0] - '0');
	}

	SDL_Log("Player has %d %d %d %d %d -> %d points",
			ctx->players[playerID].tokens[0],
			ctx->players[playerID].tokens[1],
			ctx->players[playerID].tokens[2],
			ctx->players[playerID].tokens[3],
			ctx->players[playerID].tokens[4],
			ctx->players[playerID].points);
	return 1;
}

int execTake(Context *ctx, uint8_t playerID, char *msg)
{
	int amount;

	msg++;
	SDL_Log("Player %d , takes", playerID);
	for (int i = 0; i < CARD_TYPES; i++)
	{
		amount = msg[0] - '0';
		SDL_Log("Token %d: %d", i, amount);
		ctx->players[playerID].tokens[i] += amount;
		ctx->board.tokens[i] -= amount;
		msg++;
	}
	return 1;
}
