#include "../includes/splendor.h"

int sortHandles(const void *a, const void *b)
{
	if (a == NULL || * (char * const *) a == NULL)
		return 1;
	if (b == NULL || * (char * const *) b == NULL)
		return -1;
	return SDL_strcmp(* (char * const *) a, * (char * const *) b);
}

int connect_handles(Context *ctx, char **handles, int len)
{
	int n;
	int connections;
	char msg[3];

	n = 0;
	connections = 0;
	msg[0] = 'c';
	for (int i = 0; i < MAX_PLAYERS && n < len; i++)
	{
		SDL_Log("Try connect Player id %d -> %d| handle %s =? %s", i, ctx->players[i].status, ctx->players[i].handle, handles[n]);
		if (ctx->players[i].status == DISCONNECTED)
		{
			// ctx->players[i].handle = SDL_calloc(HANDLE_LEN, sizeof(char));
			msg[1] = i + '0';
			msg[2] = '\0';
			ctx->players[i].status = CONNECTED;
			SDL_strlcpy(ctx->players[i].handle, handles[n], HANDLE_LEN);
			SDL_Log("Added player %d - %s", i, handles[n]);
			send_to(handles[n], msg);
			connections++;
			n++;
		}
		else if (!SDL_strcmp(ctx->players[i].handle, handles[n]))// In case there's a double connect with same handle
			n++;
	}
	return connections;
}

int disconnect_handles(Context *ctx, char **handles, int len)
{
	int n;
	n = 0;
	for (int i = 0; i < MAX_PLAYERS && n < len; i++)
	{
		SDL_Log("Try disconnect Player id %d | handle %s =? %s", i, ctx->players[i].handle, handles[n]);
		if (ctx->players[i].status != DISCONNECTED && !SDL_strcmp(ctx->players[i].handle, handles[n]))
		{
			SDL_Log("Remove player %d -> %s", i, handles[n]);
			// SDL_free(ctx->players[i].handle);
			ctx->players[i].status = DISCONNECTED;
			n++;
		}
	}
	return n;
}

int handle_Connect(Context *ctx, c_string_vec *new)
{
	uint8_t i;
	uint8_t n;
	uint8_t cmp;
	uint8_t newLen;
	uint8_t discLen;
	char *(newHandles[MAX_PLAYERS]) = {NULL, NULL, NULL, NULL};
	char *(discHandles[MAX_PLAYERS]) = {NULL, NULL, NULL, NULL};
	char *(old[MAX_PLAYERS]) = {
		(ctx->players[0].status == DISCONNECTED? NULL : ctx->players[0].handle),
		(ctx->players[1].status == DISCONNECTED? NULL : ctx->players[1].handle),
		(ctx->players[2].status == DISCONNECTED? NULL : ctx->players[2].handle),
		(ctx->players[3].status == DISCONNECTED? NULL : ctx->players[3].handle)
		};


	if (new->len > MAX_PLAYERS)
		return -1;
	SDL_qsort(new->ptr, new->len, sizeof(char *), sortHandles);
	SDL_qsort(old, MAX_PLAYERS, sizeof(char *), sortHandles);

	i = 0;
	n = 0;
	newLen = 0;
	discLen = 0;
	//current handles is short than handles ptr

	SDL_Log("HANDLE CONNNECT----------------");


	while (i < new->len && n < MAX_PLAYERS)
	{
		if (old[n] != NULL)
		{
			cmp = SDL_strcmp(new->ptr[i], old[n]);
		}
		else
			cmp = -1;
		if (cmp > 0) //-1 = new < old | 1 old < new
		{
			newHandles[newLen] = new->ptr[i];
			newLen++;
			i++;
		}
		else if (cmp < 0)
		{
			if (old[n] != NULL)
			{
				discHandles[newLen] = old[n];
				discLen++;
			}
			n++;
		}
		else
		{
			i++;
			n++;
		}
	}
	while (i < new->len)
	{
		newHandles[newLen] = new->ptr[i];
			newLen++;
			i++;
	}
	while (n < ctx->playerCount)
	{
		if (old[n] != NULL)
		{
			discHandles[discLen] = old[n];
			discLen++;
		}
		n++;
	}

	for (int i = 0; i < discLen; i++)
	{
		SDL_Log("DISCONNECT Handle %s", discHandles[i]);
	}
	for (int i = 0; i < newLen; i++)
	{
		SDL_Log("CONNECT Handle %s", newHandles[i]);
	}
	ctx->playerCount -= disconnect_handles(ctx, discHandles, discLen);
	ctx->playerCount += connect_handles(ctx, newHandles, newLen);

	SDL_Log("COnnect %d players", ctx->playerCount);
	for (int i = 0; i < ctx->playerCount; i++)
	{
		SDL_Log("PLayer %d| Status : %d | Handle %s", i, ctx->players[i].status, ctx->players[i].handle);
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
				ctx->players[i].status = (msg[1] + 1) - '0';
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

