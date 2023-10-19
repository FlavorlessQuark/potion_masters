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
	// else if (ctx->state == TITLE)
	// 	SDLX_TimedLoop(title_screen, ctx);
	else if (ctx->state == CONNECT_SCREEN)
		connect_screen(ctx);
	else  if (ctx->state == PLAYING)
		main_game(ctx);
}


int main_game(Context *ctx)
{
	int msgWasExec;
	char *msg;
	c_string_vec *handles;
	handles = get_connections();
	if (handles)
	{
		handle_Connect(ctx, handles);
		SDL_Log("New connections");
		if (ctx->players[0].status == CONNECTED)
			send_game_state(ctx, 0);
	}
		// SDL_Log("New connections");
	// msgWasExec = 0;
	msg = recv_from(ctx->players[ctx->turn].handle);
	if (msg)
	{
		SDL_Log("Message from %d %s", ctx->turn, msg);
		msgWasExec = execMsg(ctx, msg);
	}
	// if (msgWasExec)
	// {
	// 	send_to(ctx->players[ctx->turn].handle, "e");
	// 	ctx->turn = (ctx->turn + 1) % ctx->playerCount; // 0 if turn == playCount else turn + 1
	// 	send_game_state(ctx, ctx->turn);
	// 	SDL_Log("Now player %d turn", ctx->turn);
	// }
	for (int i = 0; i < MAX_PLAYERS; i++)
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
		// SDL_DestroyTexture(ctx->display->background);
		// ctx->display->background = NULL;
		ctx->state = CONNECT_SCREEN;
	}
}

int connect_screen(Context *ctx)
{
	c_string_vec *handles;
	uint8_t ready;
	char *msg;
	char name[10] = {"Player 0"};
	SDL_Rect src = {.x = 625, .y = 470, .w = 500, .h = 187};

	// SDL_Log("PLOSE");
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
			name[7] = i + 1 + '0';
			overlay_text(ctx->connectscreen.playerName[i].texture,ctx->assets.texUI, &src, 0x000000FF, name);
			msg = recv_from(ctx->players[i].handle);
			if (msg != NULL && msg[0] == 'r')
			{
				if (msg[1] == '1')
				{
					send_to(ctx->players[i].handle, "r");
					ctx->connectscreen.playerStatus[i]._src = (SDL_Rect){.x = 642, .y = 155, .w = 150, .h = 180};

				}
				if (msg[1] == '0')
				{
					send_to(ctx->players[i].handle, "u");
					ctx->connectscreen.playerStatus[i]._src = (SDL_Rect){.x = 490, .y = 155, .w = 150, .h = 180};
				}
				ctx->players[i].status = (msg[1] + 1) - '0';
			}
		}
		ready &= ctx->players[i].status;
	}
	// if (ready)
	// {
	// 	startGame(ctx);
	// 	ctx->state = PLAYING;
	// }
	render_connect_screen(ctx);
}
