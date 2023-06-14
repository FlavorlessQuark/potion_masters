#include "../includes/splendor.h"

int core(void *arg, char *msg)
{
	Context *ctx;

	ctx = (Context *)arg;

	if (ctx->state == EXIT_GAME)
	{
		cleanup(ctx);
		exit(0);
	}
	else if (ctx->state == TITLE)
		SDLX_TimedLoop(title_screen, ctx);
	else if (ctx->state == CONNECT_SCREEN)
		connect_screen(ctx);
	else
		main_game(ctx);
}


int main_game(Context *ctx)
{
	int msgWasExec;
	char *msg;

	msgWasExec = 0;
	msg = recv_from(ctx->players[ctx->turn].handle);
	if (msg)
	{
		SDL_Log("Message from %d %s", ctx->turn, msg);
		msgWasExec = execMsg(ctx, msg);
	}
	if (msgWasExec)
	{
		send_to(ctx->players[ctx->turn].handle, "e");
		ctx->turn = (ctx->turn + 1) % ctx->playerCount; // 0 if turn == playCount else turn + 1
		send_game_state(ctx, ctx->turn);
		SDL_Log("Now player %d turn", ctx->turn);
	}
	for (int i = 0; i < ctx->playerCount; i++)
		renderPlayer(ctx, &ctx->players[i]);

	renderBoard(ctx);
}

#define MULT 4
int title_screen(Context *ctx)
{
	if (ctx->connectscreen.counter >= 0)
	{
		if (ctx->connectscreen.counter / MULT <= 255)
		{
			SDL_SetTextureAlphaMod(ctx->display->background, ctx->connectscreen.counter / MULT);
		}
		ctx->connectscreen.counter--;
		// SDL_RenderCopy(ctx->display->renderer, ctx->di)
	}
	else
	{
		SDL_DestroyTexture(ctx->display->background);
		ctx->display->background = NULL;
		ctx->state = CONNECT_SCREEN;
	}
}

int connect_screen(Context *ctx)
{
	c_string_vec *handles;
	uint8_t ready;
	char *msg;

	handles = get_connections();
	if (handles)
		handle_Connect(ctx, handles);

	if (ctx->playerCount > 0)
		ready = READY;
	else
		ready = 0;
	for (int i = 0; i < ctx->playerCount; i++)
	{
		if (ctx->players[i].status != DISCONNECTED)
		{
			msg = recv_from(ctx->players[i].handle);
			if (msg != NULL && msg[0] == 'r')
			{
				send_to(ctx->players[i].handle, "r");
				ctx->players[i].status = (msg[1] + 1) - '0';
			}
		}
		ready &= ctx->players[i].status;
	}
	if (ready)
	{
		startGame(ctx);
		ctx->state = PLAYING;
	}
	render_connect_screen(ctx);
}
