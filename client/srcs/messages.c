#include "../includes/splendor.h"

static char msg[MSG_LEN];

// msg = "Player-Action"
// Action -> Reserve : r[card-id]|
// 		  -> Pay : p|[isReserveCard][card_id]|[tok1, tok2, tok3, tok4, tok5]
//        -> Take : t[tok1, tok2, tok3, tok4]
//Example : 1r12 , 2p042|10201, 4t00111, 2p142|10201

void sendReserve(Context *ctx)
{
	int i;
	SDL_Rect dst;

	SDL_memset(msg, 0, MSG_LEN);

	msg[0] = ctx->player.id + '0';
	msg[1] = 'r';
	for (i = 0; i < CARD_ID_LEN; i++)
		msg[2 + i] = ctx->buyscreen.selected->id[i];
	// msg[2] = '0';
	msg[2 + i] = '\0';
	memcpy(ctx->player.reserved[ctx->player.reserveCount].id, ctx->buyscreen.selected->id, CARD_ID_LEN);
	memcpy(ctx->player.reserved[ctx->player.reserveCount].cost, ctx->buyscreen.selected->cost, TOK_COUNT - 1);

	// ctx->player.reserved[ctx->player.reserveCount].sprite.texture =
	ctx->player.reserved[ctx->player.reserveCount].sprite._src =  ctx->buyscreen.selected->sprite._src;
	ctx->player.reserved[ctx->player.reserveCount].sprite.src =  &ctx->player.reserved[ctx->player.reserveCount].sprite._src;
	ctx->player.reserveCount++;
	ctx->player.tokens[CARD_TYPES]++;

	sendMessage(msg);
}

void sendPay(Context *ctx)
{
	int i;
	int n;
	SDL_memset(msg, 0, MSG_LEN);

	msg[0] = ctx->player.id + '0';
	msg[1] = 'p';
	msg[2] = ctx->buyscreen.cardOrigin + '0';
	for (i = 0; i < CARD_ID_LEN; i++)
		msg[3 + i] = ctx->buyscreen.selected->id[i];
	if (msg[3 + i - 1] != '|')
		msg[3 + i - 1] = '|';
	n = 3 + i;
	for (i = 0; i < TOK_COUNT; i++)
	{
		msg[n + i] = ctx->player.tokens[i] + '0';
	}
	msg[n + i] = ctx->buyscreen.selected->points + '0';
	msg[n + i + 1] = '\0';

	if (ctx->buyscreen.cardOrigin)
		delReserved(ctx, ctx->buyscreen.selected->_id);
	// SDL_Log("Sending %s -> %s", msg, ctx->buyscreen.selected->id);
	sendMessage(msg);
}

void sendTakeTokens(Context *ctx, uint8_t *taken)
{
	SDL_memset(msg, 0, MSG_LEN);

	msg[0] = ctx->player.id + '0';
	msg[1] = 't';
	msg[2] = taken[TOK_A] + '0';
	msg[3] = taken[TOK_B] + '0';
	msg[4] = taken[TOK_C] + '0';
	msg[5] = taken[TOK_D] + '0';
	msg[6] = taken[TOK_R] + '0';
	msg[7] = '\0';
	sendMessage(msg);
}

// b[tok1, tok2, tok3, tok4, tok5]|[r0c0 ID] ||[r0c1 ID]| |[r0c2 ID] ...
void parseMsg(Context *ctx, char *input)
{
	SDL_Log("Parsin msg %s", input);
	if (input[0] == 'c')
	{
		ctx->player.id = input[1] -'0';
		ctx->connection.status = CONNECTED;
		SDL_Log("Connected as Player %d", ctx->player.id);
	}
	else if (input[0] == 'r')
	{
		ctx->connection.status = CONNECTEDCONSOLE;
	}
	else if (input[0] == 'd')
	{

		if(input[1] == 'c')
			ctx->connection.status = CONNECTED;
		if(input[1] == 's')
			ctx->connection.status = DISCONNECTED;
	}
	else if (input[0] == 's')
	{
		ctx->state = PLAYERSTATUS;
		SDL_Log("Game starting");
	}
	else if (input[0] == 'e')
	{
		SDL_Log("Turn ending");
		endTurn(ctx);
	}
	else if (input[0] == 'b')
	{
		uint8_t offset;
		int _id;
		char *id;

		// SDL_Log("Received board state %s | It's my turn!", input);
		ctx->state = 0;
		ctx->board.tokens[0] = input[1] - '0';
		ctx->board.tokens[1] = input[2] - '0';
		ctx->board.tokens[2] = input[3] - '0';
		ctx->board.tokens[3] = input[4] - '0';
		ctx->board.tokens[4] = input[5] - '0';
		offset = 6;
		for (int r = 0; r < ROW_COUNT; r++)
		{
			for (int c = 0; c < MAX_ROWCARD; c++)
			{
				id = input + offset;
				offset += extract_num(id, &_id) + 1;
				// SDL_Log("ID %d", _id);
				// SDL_Log("AFTER %s", input + offset);
				if (ctx->board.rows[r].revealed[c]._id != _id)
					fillCard(&ctx->board.rows[r].revealed[c], _id, id);
			}
		}
		startTurn(ctx);
	}
	ctx->connection.hasMessage = SDL_FALSE;

}
