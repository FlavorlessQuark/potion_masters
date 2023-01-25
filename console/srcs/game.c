#include "../includes/splendor.h"

int connectScreen(Context *ctx)
{
	static uint8_t ready[MAX_PLAYERS];
	c_string_vec *handles;

	handles = getConnections();
	ctx->playerCount = handles->len;
	renderConnectScreen(ctx, ready);
}

int mainGame(Context *ctx)
{
	static int turn;
	int msgWasExec;
	char *msg;

	msg = recvFrom(turn);
	if (msg)
		msgWasExec = execMsg(ctx, msg);
	if (msgWasExec)
		turn = (turn + 1) % ctx->playerCount; // 0 if turn == playCount else turn + 1
		//render board
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
		connectScreen(ctx);
	else
		mainGame(ctx);

}
