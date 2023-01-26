#include "../includes/splendor.h"

int mainGame(Context *ctx)
{
	static int turn;
	int msgWasExec;
	char *msg;

	msg = recv_from(turn);
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
		connect_screen(ctx);
	else
		mainGame(ctx);

}
