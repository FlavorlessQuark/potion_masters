#include "../includes/splendor.h"

void nextTurn(Context *ctx)
{
	int i;

	i = (ctx->turn + 1) % MAX_PLAYERS;
	while (i != ctx->turn)
	{
		if (ctx->players[i].status == READY)
		{
			ctx->turn = i;
			break ;
		}
		i = (i + 1) % MAX_PLAYERS;
	}
	sendBoardState(ctx, ctx->turn);
}

void startGame(Context *ctx)
{
	ctx->turn = 0;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ctx->players[i].status == READY)
			send_to(ctx->players[i].handle, "s");
	}
	sendBoardState(ctx, ctx->turn);
}

int mainGame(Context *ctx)
{
	int msgWasExec;
	char *msg;

	msg = recv_from(ctx->players[ctx->turn].handle);
	if (msg)
		SDL_Log("Message from %d %s", ctx->turn, msg);
		msgWasExec = execMsg(ctx, msg);
	// if (msgWasExec)
	// 	turn = (turn + 1) % ctx->playerCount; // 0 if turn == playCount else turn + 1
	renderPlayer(&ctx->board.playerUI[0]);
	renderPlayer(&ctx->board.playerUI[1]);
	renderPlayer(&ctx->board.playerUI[2]);
	renderPlayer(&ctx->board.playerUI[3]);
	renderBoard(ctx);
}

int core(void *arg, char *msg)
{
	Context *ctx;
	ctx = (Context *)arg;

	if (ctx->state == EXIT_GAME)
	{
		cleanup(ctx);
		exit(0);
	}
	else if (ctx->state == CONNECT_SCREEN)
		connect_screen(ctx);
	else
		mainGame(ctx);
}
