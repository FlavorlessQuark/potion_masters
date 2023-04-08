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

void get_current_handles(void)
{
	free_strvec(handles);
	CP_CHECK(get_client_handles(&handles));
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
	char msg[5];

	msg[0] = 'c';
	ctx->players[playerID].status |= CONNECTED;
	SDL_strlcpy(ctx->players[playerID].handle, handle, HANDLE_LEN);
	SDL_Log("Connected player %d - %s", playerID, handle);
	msg[1] = playerID + '0';
	msg[2] = ctx->players[playerID].status + '0';
	msg[3] = ctx->state + '0';
	msg[4] = '\0';
	send_to(handle, msg);
}

# define INTER 5


int connect_handles_filtered(Context *ctx)
{
	int i;
	uint8_t oldState[MAX_PLAYERS];

	for ( i = 0; i < MAX_PLAYERS; i++)
	{
		oldState[i] = ctx->players[i].status;
		ctx->players[i].status = DISCONNECTED;
	}
	for (int n = 0; n < handles.len; n++)
	{
		for ( i = 0; i < MAX_PLAYERS; i++)
		{
			if (!SDL_strcmp(ctx->players[i].handle, handles.ptr[n])
			)
			{
				if (oldState[i] == DISCONNECTED)
				{
					connect_player(ctx, i, handles.ptr[n]);
				}
				else
					ctx->players[i].status = oldState[i];
				send_player_state(ctx, i);
				if (ctx->turn == i)
					send_game_state(ctx, i);
				break ;
			}
		}
	}
	ctx->playerCount = MIN(MAX_PLAYERS, handles.len);
}

int connect_handles_unfiltered(Context *ctx)
{
	uint8_t oldState[MAX_PLAYERS];
	char 	*new_handles[MAX_PLAYERS];
	int 	newlen;
	int 	i;

	newlen = 0;
	for ( i = 0; i < MAX_PLAYERS; i++)
	{
		oldState[i] = ctx->players[i].status;
		ctx->players[i].status = DISCONNECTED;
	}
	for (int n = 0; n < handles.len; n++)
	{
		for ( i = 0; i < MAX_PLAYERS; i++)
		{
			if (!SDL_strcmp(ctx->players[i].handle, handles.ptr[n]))
			{
				if (oldState[i] == DISCONNECTED)
					connect_player(ctx, i, handles.ptr[n]);
				else
					ctx->players[i].status = oldState[i];
				break ;
			}
		}
		if (i == MAX_PLAYERS)
		{
			new_handles[newlen] = handles.ptr[n];
			newlen++;
		}
	}
	for (int n = 0; n < newlen; n++)
	{
		for ( i = 0; i < MAX_PLAYERS; i++)
		{
			if (ctx->players[i].status == DISCONNECTED)
			{
				connect_player(ctx, i, new_handles[n]);
				break ;
			}
		}
	}
	ctx->playerCount = MIN(MAX_PLAYERS, handles.len);
}
