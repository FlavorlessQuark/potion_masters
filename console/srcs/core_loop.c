#include "../includes/splendor.h"

#define WAIT_TIME (10 * (FPS * UPDATE_LEN_MS))
#define CONNECTION_TIMER (2 * (FPS * UPDATE_LEN_MS))
#define MIN_PLAYERS 2

int timer_fn(int *wait)
{
	*wait -= 1;
	return 0;
}


int core(void *arg, char *msg)
{
	Context *ctx;
	ctx = (Context *)arg;

	if (ctx->state == CONNECT_SCREEN)
		connect_screen(ctx);
	else if (ctx->state == TITLE)
		SDLX_TimedLoop(title_screen, ctx);
	else if (ctx->state == PLAYING)
		main_game(ctx);
}

int main_game(Context *ctx)
{
	int msgWasExec;
	static int time;

	if (get_connections() != NULL)
		connect_handles_filtered(ctx);
	msgWasExec = recv_from(ctx, ctx->turn, ctx->players[ctx->turn].handle);
	if (msgWasExec > 0)
	{
		send_player_state(ctx, ctx->turn);
		ctx->turn = (ctx->turn + 1) % ctx->playerCount; // 0 if turn == playCount else turn + 1
		send_game_state(ctx, ctx->turn);
		SDL_Log("Now player %d turn", ctx->turn);
	}
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (ctx->players[i].status >= CONNECTED)
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
		SDL_DestroyTexture(ctx->display->background);
		ctx->connectscreen.counter = WAIT_TIME;
		ctx->display->background = NULL;
		ctx->state = CONNECT_SCREEN;
		get_current_handles();
		connect_handles_unfiltered(ctx);
	}
}



int connect_screen(Context *ctx)
{
	uint8_t ready;

	if (get_connections() != NULL)
		connect_handles_unfiltered(ctx);

	if (ctx->playerCount > 0)
		ready = READY;
	else
		ready = 0;
	for (int i = 0; i < ctx->playerCount; i++)
	{
		if (ctx->players[i].status != DISCONNECTED)
		{
			recv_from(ctx, i, ctx->players[i].handle);
			// SDL_Log("PLaery %d Ready %d",i,  ctx->players[i].status);
			ready &= ctx->players[i].status;
		}
	}
	// if (ready && ctx->playerCount >= MIN_PLAYERS)
	// SDL_Log("Ready %d", ready);
	if (ready == READY)
	{
		if (ctx->connectscreen.counter <= 0)
		{
			startGame(ctx);
		}
		else
			SDLX_TimedLoop(timer_fn, &ctx->connectscreen.counter);
		render_connect_screen(ctx, &ctx->connectscreen.counter);
	}
	else
	{
		render_connect_screen(ctx, NULL);
	}
}
