#include "../includes/splendor.h"
#include <time.h>

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
	msg = recv_from(ctx->players[ctx->turn].handle);
	if (msg)
	{
		SDL_Log("Message from %d %s", ctx->turn, msg);
		msgWasExec = execMsg(ctx, msg);
		if (msgWasExec)
		{
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				if (ctx->players[i].status != DISCONNECTED)
				{
					send_game_state(ctx, ctx->turn);
				}
			}
		}
	}
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ctx->players[i].status != DISCONNECTED)
			renderPlayer(ctx, &ctx->players[i]);
	}

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

#define WAIT_TIME 1

int connect_screen(Context *ctx)
{
	static time_t timer;
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
					ctx->players[i].status = READY;
					ctx->connectscreen.playerStatus[i]._src = (SDL_Rect){.x = 642, .y = 155, .w = 150, .h = 180};
				}
				if (msg[1] == '0')
				{
					send_to(ctx->players[i].handle, "u");
					ctx->players[i].status = CONNECTED;
					ctx->connectscreen.playerStatus[i]._src = (SDL_Rect){.x = 490, .y = 155, .w = 150, .h = 180};
				}
				ctx->players[i].status = (msg[1] + 1) - '0';
			}
		}
		ready &= ctx->players[i].status;
	}
	if (ready)
	{
		int diff = (int)difftime(time(NULL), timer);
		char text[20];

		sprintf(text, "Game starts in: %d", WAIT_TIME - diff);
		if (diff >= WAIT_TIME)
		{
			startGame(ctx);
			ctx->state = PLAYING;
		}
		overlay_text(ctx->connectscreen.timer.texture, NULL, NULL, 0xFFFFFFFF, text);
	}
	else
	{
		overlay_text(ctx->connectscreen.timer.texture, NULL, NULL, 0xFFFFFFFF, "Waiting...");
		time(&timer);
	}
	render_connect_screen(ctx);
}
