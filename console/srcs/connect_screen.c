#include "../includes/splendor.h"

int sortHandles(const void *a, const void *b)
{
	if (a == NULL || * (char * const *) a == NULL)
		return 1;
	if (b == NULL || * (char * const *) b == NULL)
		return -1;
	return SDL_strcmp(* (char * const *) a, * (char * const *) b);
}

void connect_handles(Context *ctx, char **handles, int len)
{
	int n;

	n = 0;
	for (int i = 0; i < MAX_PLAYERS && n < len; i++)
	{
		if (ctx->players[i].handle == NULL)
		{
			ctx->players[i].handle = SDL_calloc(HANDLE_LEN, sizeof(char));
			SDL_strlcpy(ctx->players[i].handle, handles[n], HANDLE_LEN);
			SDL_Log("Added player %d - %s", i, handles[n]);
			n++;
		}
	}
}

void disconnect_handles(Context *ctx, char **handles, int len)
{
	int n;

	n = 0;
	for (int i = 0; i < MAX_PLAYERS && n < len; i++)
	{

		if (ctx->players[i].handle != NULL && !SDL_strcmp(ctx->players[i].handle, handles[n]))
		{
			SDL_Log("Remove player %d - %s", i, handles[n]);
			SDL_free(ctx->players[i].handle);
			ctx->players[i].handle = NULL;
			n++;
		}
	}
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
		ctx->players[0].handle,
		ctx->players[1].handle,
		ctx->players[2].handle,
		ctx->players[3].handle
		};


	if (new->len > MAX_PLAYERS)
		return -1;
	SDL_qsort(new->ptr, new->len, sizeof(char *), sortHandles);
	SDL_qsort(old,MAX_PLAYERS, sizeof(char *), sortHandles);

	i = 0;
	n = 0;
	newLen = 0;
	//current handles is short than handles ptr


	while (i < new->len && n < MAX_PLAYERS)
	{
		if (old[n] != NULL)
		{
			cmp = SDL_strcmp(new->ptr[i], old[n]);
		}
		else
		{
			cmp = -1;
		}
		if (cmp > 0) //-1 = new < old | 1 old < new
		{
			newHandles[newLen] = new->ptr[i];
			SDL_Log("Should connect %s", new->ptr[i]);
			newLen++;
			i++;
		}
		else if (cmp < 0)
		{
			discHandles[newLen] = old[n];
			SDL_Log("Should disc %s", old[n]);
			discLen++;
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
		SDL_Log("Should connect %s", new->ptr[i]);
			newLen++;
			i++;
	}
	while (n < ctx->playerCount)
	{
		discHandles[newLen] = old[n];
		SDL_Log("Should disc %s", old[n]);
		discLen++;
		n++;
	}


	disconnect_handles(ctx, discHandles, discLen);
	connect_handles(ctx, newHandles, newLen);

	ctx->playerCount = new->len;
}


int connect_screen(Context *ctx)
{
	static uint8_t ready[MAX_PLAYERS]= {-1, -1, -1, -1};
	c_string_vec *handles;
	char *msg;

	handles = get_connections();
	if (handles)
		handle_Connect(ctx, handles);

	for (int i = 0; i < ctx->playerCount; i++)
	{
		msg = recv_from(i);
		if (msg != NULL && msg[0] == 'r')
			ready[i] = msg[1] - '0';
	}
	render_connect_screen(ctx, ready);
}

void render_connect_screen(Context *ctx, uint8_t ready[MAX_PLAYERS])
{
	int space;
	SDL_Rect rect;

	rect.h = ctx->display->win_h / 3;
	rect.w = ctx->display->win_w / 5;
	rect.y = ctx->display->win_h / 3;
	rect.x = ctx->display->win_w / 10;
	space = rect.x;

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		if (i >= ctx->playerCount)
			SDL_SetRenderDrawColor(ctx->display->renderer, 255, 0x0, 0x0,255);
		else if (ready[i])
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 255, 0x0,255);
		else
			SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 255, 255);
		SDL_RenderDrawRect(ctx->display->renderer, &rect);
		rect.x += space + rect.w;
	}
	SDL_SetRenderDrawColor(ctx->display->renderer, 0x0, 0x0, 0x0, 255);
}
