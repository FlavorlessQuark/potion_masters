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
	SDL_Texture *tmp;

	SDL_memset(msg, 0, MSG_LEN);

	msg[0] = ctx->player.id + '0';
	msg[1] = 'r';
	for (i = 0; i < CARD_ID_LEN; i++)
		msg[2 + i] = ctx->buyscreen.selected->id[i];
	// msg[2] = '0';
	msg[2 + i] = '\0';
	memcpy(ctx->player.reserved[ctx->player.reserveCount].id, ctx->buyscreen.selected->id, CARD_ID_LEN);
	memcpy(ctx->player.reserved[ctx->player.reserveCount].cost, ctx->buyscreen.selected->cost, TOK_COUNT - 1);
	tmp = ctx->player.reserved[ctx->player.reserveCount].sprite.texture;
	ctx->player.reserved[ctx->player.reserveCount].sprite.texture = ctx->buyscreen.selected->sprite.texture;
	ctx->buyscreen.selected->sprite.texture = tmp;
	// ctx->player.reserved[ctx->player.reserveCount].sprite.texture =

	ctx->player.reserveCount++;
	ctx->player.tokens[TOK_COUNT - 1]++;

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
	SDL_Log("Pay for %s", ctx->buyscreen.selected->id);
	memcpy(msg + 3, ctx->buyscreen.selected->id, CARD_ID_LEN);
	if (ctx->buyscreen.cardOrigin)
		delReserved(ctx, ctx->buyscreen.selected->_id);
	msg[3 + CARD_ID_LEN] = '\0';
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

void parse_board_state(Context *ctx, char *input);
void parse_player_state(Context *ctx, char *input);

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
		parse_player_state(ctx, input + 1);
	}
	else if (input[0] == 'b')
	{
		parse_board_state(ctx, input + 1);
	}
	ctx->connection.hasMessage = SDL_FALSE;

}


void parse_board_state(Context *ctx, char *input)
{
	uint8_t offset;
	int _id;
	int i;

	SDL_Log("Received board state %s", input);
	ctx->state = PLAYERSTATUS;
	for (i = 0; i < TOK_COUNT; i++)
		ctx->board.tokens[i] = input[i] - '0';
	input += i;
	for (int r = 0; r < ROW_COUNT; r++)
	{
		for (int c = 0; c < MAX_ROWCARD; c++)
		{
			// memcpy(&ctx->board.rows[r].revealed[c].id, input, CARD_ID_LEN);
			extract_card_from_input(ctx, &ctx->board.rows[r].revealed[c], input);
			// extract_num(ctx->board.rows[r].revealed[c].id, &_id);
			// if (ctx->board.rows[r].revealed[c]._id != _id)
			// {
			// 	fillCard(&ctx->board.rows[r].revealed[c]);
			// 	generateCardTexture(ctx->cardTex, &ctx->board.rows[r].revealed[c], ctx->board.rows[r].revealed[c].id[1] - '0');
			// }
			input += CARD_ID_LEN;
		}
	}
	startTurn(ctx);
}

void parse_player_state(Context *ctx, char *input)
{
	int i;
	int reserveCount;

	SDL_Log("Received player status %s", input);
	// input++;
	// ctx->state = PLAYERSTATUS;
	// for (i = 0; i < TOK_COUNT; i++)
	// 	ctx->player.tokens[i] = input[i] - '0';
	// input += i;
	// reserveCount =  *input - '0';
	// for (i = 0; i < reserveCount; i++)
	// {
	// 	memcpy(&ctx->board.rows[r].revealed[c].id, input, CARD_ID_LEN);
	// 	extract_num(ctx->board.rows[r].revealed[c].id, &_id);
	// }
	endTurn(ctx);
}
