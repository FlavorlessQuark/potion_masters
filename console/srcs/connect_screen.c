#include "../includes/splendor.h"

int sortHandles(const void *a, const void *b)
{
	if (a == NULL || * (char * const *) a == NULL)
		return 1;
	if (b == NULL || * (char * const *) b == NULL)
		return -1;
	return SDL_strcmp(* (char * const *) a, * (char * const *) b);
}

int handle_Connect(Context *ctx, c_string_vec *handles)
{
	uint8_t n;
	uint8_t newLen;
	uint8_t nextId;
	char *(newHandles[]) = {NULL, NULL, NULL, NULL};
	char *(currentHandles[]) = {
		ctx->players[0].handle,
		ctx->players[1].handle,
		ctx->players[2].handle,
		ctx->players[3].handle
		};

	SDL_qsort(handles->ptr, handles->len, sizeof(char *), sortHandles);
	SDL_qsort(currentHandles,MAX_PLAYERS, sizeof(char *), sortHandles);

	n = 0;
	newLen = 0;
	//current handles is short than handles ptr
	for (int i = 0; i < handles->len; i++)
	{
		if (currentHandles[n] != NULL )
		if (currentHandles[n] == NULL || SDL_strcmp(handles->ptr[i], currentHandles[n]))
		{
			newHandles[newLen] = handles->ptr[i];
			newLen++;
		}
		else
			n++;
	}

	if (handles->len >= MAX_PLAYERS)
		return -1;
	n = 0;
	for (int i = 0; i < MAX_PLAYERS && n < newLen; i++)
	{
		if (ctx->players[i].handle == NULL)
		{
			ctx->players[i].handle = SDL_calloc(HANDLE_LEN, sizeof(char));
			SDL_strlcpy(ctx->players[i].handle, newHandles[n], HANDLE_LEN);
			SDL_Log("Added player %d", i);
			n++;
		}
	}
	ctx->playerCount = handles->len;

	// nextId = 0;
	// while (nextId < MAX_PLAYERS && ctx->players[nextId].handle != NULL)
	// 	nextId ++;



}

int handle_Disconnect(Context *ctx, c_string_vec *handles)
{

}

int connect_screen(Context *ctx)
{
	static uint8_t ready[MAX_PLAYERS]= {-1, -1, -1, -1};
	c_string_vec *handles;
	char *msg;

	handles = get_connections();
	if (handles)
	{
		if (ctx->playerCount < handles->len)
			handle_Connect(ctx, handles);
		else
			handle_Disconnect(ctx, handles);
	}

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
