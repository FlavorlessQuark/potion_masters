#include "../includes/splendor.h"

#define NUMS "0123456789"
static char msg[20];


// msg = "Player-Action"
// Action -> Reserve : r[card-id]|
// 		  -> Pay : p|[isReserveCard][card_id]|[tok1, tok2, tok3, tok4, tok5] row is always 0 if reserved card
//        -> Take : t[tok1, tok2, tok3, tok4]
//Example : 1r12 , 2p042|10201, 4t00111, 2p142|10201

int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}


void composeReserve(Context *ctx)
{
	SDL_memset(msg, 0, 20);

	msg[0] = ctx->player.id + '0';
	msg[1] = 'r';
	msg[2] = '0';
	msg[3] = '\0';
	SDL_Log("MEssage %s", msg);
}

void composePay(Context *ctx)
{
	SDL_memset(msg, 0, 20);

	msg[0] = ctx->player.id + '0';
	msg[1] = 'p';
	msg[2] = '0';
	msg[3] = '|';
	msg[4] = ctx->buyscreen->cardOrigin + '0';
	msg[5] = ctx->player.tokens[TOK_A] + '0';
	msg[6] = ctx->player.tokens[TOK_B] + '0';
	msg[7] = ctx->player.tokens[TOK_C] + '0';
	msg[8] = ctx->player.tokens[TOK_D] + '0';
	msg[9] = ctx->player.tokens[TOK_R] + '0';
	msg[10] = '\0';
	SDL_Log("MEssage %s", msg);
}

void composeTakeTokens(Context *ctx)
{
	SDL_memset(msg, 0, 20);

	msg[0] = ctx->player.id + '0';
	msg[1] = 't';
	msg[2] = ctx->player.tokens[TOK_A] + '0';
	msg[3] = ctx->player.tokens[TOK_B] + '0';
	msg[4] = ctx->player.tokens[TOK_C] + '0';
	msg[5] = ctx->player.tokens[TOK_D] + '0';
	msg[6] = ctx->player.tokens[TOK_R] + '0';
	msg[7] = '\0';
	SDL_Log("MEssage %s", msg);
}

// b[tok1, tok2, tok3, tok4, tok5]|[r0c0 ID] ||[r0c1 ID]| |[r0c2 ID] ...
void parseMsg(Context *ctx, char *input)
{
	if (input[0] == 'c')
	{
		ctx->player.id = input[1] -'0';
		ctx->connection.status = CONNECTEDCONSOLE;
		SDL_Log("Connected as Player %d", ctx->player.id);
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
	else if (input[0] == 'b')
	{
		uint8_t offset;
		uint8_t id;


		ctx->board.tokens[0] = input[1] - '0';
		ctx->board.tokens[1] = input[2] - '0';
		ctx->board.tokens[2] = input[3] - '0';
		ctx->board.tokens[3] = input[4] - '0';
		ctx->board.tokens[4] = input[5] - '0';
		offset = 7;
		for (int r = 0; r < ROW_COUNT; r++)
		{
			for (int c = 0; c < MAX_ROWCARD; c++)
			{
				offset += extract_num(input + offset, &id);
				ctx->board.rows[r].revealed[c] = &ctx->cards[id];
			}
		}
	}
	ctx->connection.hasMessage = SDL_FALSE;
}
