#include "../includes/splendor.h"

#define MSG_LEN (TOK_COUNT * 3) + (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) + (MAX_RESERVE * CARD_ID_LEN + 1) + 1 + 3
// (TOK_COUNT * 3)  \ /// Board tokens + player permanent & tokens
// + (ROW_COUNT * (CARD_ID_LEN + 1) * MAX_ROWCARD) \ // Board card Ids (+1 for separator)
// + (MAX_RESERVE * CARD_ID_LEN + 1) \ // Player Reserved card iDs (+1 for separator)
// +  1 \ // Player points
// + 3 \ // Null terminator, type prefix and player ID
char message[MSG_LEN];

// b[boardTokens]|[playerTokens][playerPoints][r0c0 ID] ||[r0c1 ID]| |[r0c2 ID] ...|[reservedIDs]
int send_game_state(Context *ctx, int player)
{
	int offset;

	offset = 0;
	message[0] = 'b';
	for (int i = 0; i < TOK_COUNT; i++)
		message[++offset] = ctx->board.tokens[i] + '0';
	offset++;
	for (int i = 0; i < ROW_COUNT; i++)
	{
		for (int x = 0; x < MAX_ROWCARD; x++)
		{
			SDL_memcpy(message + offset, ctx->board.rows[i].revealed[x].id, CARD_ID_LEN);
			offset += CARD_ID_LEN;
		}
	}
	message[offset + 1] = '\0';
	SDL_Log("Send State %s  to %d", message, player);
	send_to(ctx->players[player].handle, message);
}

int send_player_state(Context *ctx, int player)
{
	int offset;

	offset = 0;
	message[0] = 'e';
	for (int i = 0; i < TOK_COUNT; i++)
		message[++offset] = ctx->players[player].tokens[i] + '0';
	message[++offset] = ctx->players[player].reserveCount + '0';
	// for (int i = 0; i < ctx->players[player].reserveCount; i++)
	// {
	// 	for (int s = 0; ctx->board.rows[i].revealed[x].id[s] != '\0'; s++)
	// 		message[++offset] = ctx->board.rows[i].revealed[x].id[s];
	// }
	message[offset + 1] = '\0';
	SDL_Log("Send State %s  to %d", message, player);
	send_to(ctx->players[player].handle, message);
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
	SDL_Texture *tmp;

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

	tmp = ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite.texture ;
	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount]._id = card->_id;
	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].points = card->points;
	ctx->players[playerID].reserved[ctx->players[playerID].reserveCount].sprite.texture = card->sprite.texture;
	card->sprite.texture = tmp;

	ctx->players[playerID].tokens[TOK_R]++;
	ctx->players[playerID].reserveCount++;

	if (ctx->board.rows[id[0] - '0'].remainCount > 0)
	{
		generateCard(ctx->Tcards, card, id[0] - '0');
		SDLX_SpritePrint(&card->sprite);
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
	int tmp;
	Card *card;

	msg++;
	isReserved = msg[0] - '0';
	msg++;
	id = msg;
	extract_num(msg, &cardId) + 1;

	SDL_Log("Player %d , Buys card %d from reserve? %d | msg %s", playerID, cardId, isReserved, msg);
	SDL_Log("CArd %s points %c", msg, msg[CARD_ID_LEN - 1]);
	for (int i = 0; i < CARD_TYPES; i++)
	{
		SDL_Log("Paying token %d (%d) my tokens %d Owned %d",
			i, id[i + 3] - '0',
			ctx->players[playerID].tokens[i],
			ctx->players[playerID].owned[i]
		);
		amount = (id[i + 3] - '0' );
		amount = amount - ctx->players[playerID].owned[i];
		if (amount > 0)
		{
			// SDL_
			if (amount > ctx->players[playerID].tokens[i])
			{
				ctx->board.tokens[i] += ctx->players[playerID].tokens[i];
				amount -= ctx->players[playerID].tokens[i];
				ctx->players[playerID].tokens[i] = 0;
				ctx->players[playerID].tokens[TOK_R] -= amount;
				ctx->board.tokens[TOK_R] += amount;
			}
			else{
				ctx->players[playerID].tokens[i] -= amount;
				ctx->board.tokens[i] += amount;
			}
		}
	}

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
	SDL_Log("Card type %d", id[1] - '0');
	ctx->players[playerID].owned[id[1] - '0']++;
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
		ctx->players[playerID].tokens[i] += amount;
		ctx->board.tokens[i] -= amount;
		msg++;
	}
	return 1;
}
