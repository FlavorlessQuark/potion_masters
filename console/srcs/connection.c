#include "../includes/splendor.h"
#include <stdbool.h>
#define SUCCESS 0;
#define FAILURE 1;
typedef uint64_t Err;
static c_string_vec handles = {0};
static c_string_vec msg;
extern void free_strvec(c_string_vec vec);
extern Err clients_changed(bool *did_change);
extern Err get_client_handles(c_string_vec *client_handles);
extern Err send_message(char *client, char *msg);
extern Err get_messages(char *client, c_string_vec *messages);

// control pad check
#define CP_CHECK(x)                                                        \
	do                                                                     \
	{                                                                      \
		uint64_t retval = (x);                                             \
		if (retval != 0)                                                   \
		{                                                                  \
			fprintf(stderr, "controlpads error: %s returned %ld as %s:%d", \
					#x, retval, __FILE__, __LINE__);                       \
		}                                                                  \
	} while (0)

c_string_vec *get_connections(void)
{
	bool did_change;

	CP_CHECK(clients_changed(&did_change));
	if (did_change)
	{
		free_strvec(handles);
		CP_CHECK(get_client_handles(&handles));
	}
	else
		return NULL;
	return &handles;
}

void  parse_connections(Context *ctx)
{
	c_string_vec *handles;

	handles = get_connections();
	if (handles)
	{
		SDL_Log("New connections");
		for (int i; i < handles->len ; i++)
		{
			SDL_Log("Got handle %s", handles->ptr[i]);
		}
		handle_Connect(ctx, handles);
	}
}

int recv_from(Context *ctx, char *handle)
{
	int result;

	CP_CHECK(get_messages(handle, &msg));
	result = msg.len;
	for (int i = 0; i < msg.len; i++)
		result = execMsg(ctx, msg.ptr[i]);
	free_strvec(msg);
	return result;
}

int send_to(char *handle, char *msg)
{
	CP_CHECK(send_message(handle, msg));
}

int sortHandles(const void *a, const void *b)
{
	if (a == NULL || *(char *const *)a == NULL)
		return 1;
	if (b == NULL || *(char *const *)b == NULL)
		return -1;
	return SDL_strcmp(*(char *const *)a, *(char *const *)b);
}

void connect_player(Context *ctx, int playerID, char *handle)
{
	char msg[4];

	msg[0] = 'c';
	ctx->players[playerID].status = CONNECTED;
	SDL_strlcpy(ctx->players[playerID].handle, handle, HANDLE_LEN);
	SDL_Log("Connected player %d - %s", playerID, handle);
	msg[1] = playerID + '0';
	msg[2] = ctx->players[playerID].status + '0';
	msg[3] = '\0';
	send_to(handle, msg);
}

int connect_handles(Context *ctx, char **handles, int len)
{
	int n;
	int connections;


	n = 0;
	connections = 0;
	if (ctx->state == PLAYING)
	{
		for (int n = 0; n < len; n++)
		{
			// SDL_Log("Try reconnect Player id %d -> %d| handle %s =? %s", i, ctx->players[i].status, ctx->players[i].handle, handles[n]);

			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				if (!SDL_strcmp(ctx->players[i].handle, handles[n]))
				{
					connect_player(ctx, i, handles[n]);
					connections++;
					break ;
				}
			}
		}
	}
	else
		for (int i = 0; i < MAX_PLAYERS && n < len; i++)
		{
			// SDL_Log("Try connect Player id %d -> %d| handle %s =? %s", i, ctx->players[i].status, ctx->players[i].handle, handles[n]);
			if (ctx->players[i].status == DISCONNECTED)
			{
				// ctx->players[i].handle = SDL_calloc(HANDLE_LEN, sizeof(char));
				connect_player(ctx, i, handles[n]);
				connections++;
				n++;
			}
			else if (!SDL_strcmp(ctx->players[i].handle, handles[n])) // In case there's a double connect with same handle
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
		// SDL_Log("Try disconnect Player id %d | handle %s =? %s", i, ctx->players[i].handle, handles[n]);
		if (ctx->players[i].status != DISCONNECTED && !SDL_strcmp(ctx->players[i].handle, handles[n]))
		{
			SDL_Log("Disconnected player %d -> %s", i, handles[n]);
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
		(ctx->players[0].status == DISCONNECTED ? NULL : ctx->players[0].handle),
		(ctx->players[1].status == DISCONNECTED ? NULL : ctx->players[1].handle),
		(ctx->players[2].status == DISCONNECTED ? NULL : ctx->players[2].handle),
		(ctx->players[3].status == DISCONNECTED ? NULL : ctx->players[3].handle)};

	if (new->len > MAX_PLAYERS)
		return -1;
	SDL_qsort(new->ptr, new->len, sizeof(char *), sortHandles);
	SDL_qsort(old, MAX_PLAYERS, sizeof(char *), sortHandles);

	i = 0;
	n = 0;
	newLen = 0;
	discLen = 0;
	// current handles is short than handles ptr

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

	SDL_Log("Connected %d players", ctx->playerCount);
	for (int i = 0; i < ctx->playerCount; i++)
	{
		SDL_Log("Player %d| Status : %d | Handle %s", i, ctx->players[i].status, ctx->players[i].handle);
	}
	SDL_Log("End connect");
}
