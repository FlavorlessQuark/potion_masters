#include "../includes/splendor.h"
#include <emscripten.h>
static Context *ctx;

void recvMessage(char *msg)
{
	SDL_Log("received message");
	ctx->connection.hasMessage = SDL_TRUE;
	ctx->connection.message = msg;
}

void setConnectionStatus(int status)
{
	ctx->connection.status = status;
}

void initConnection(Context *context)
{
	ctx = context;
	context->state = CONNECT;
	EM_ASM({start_connect()});
}

void sendPlayerTurn()
{
	EM_ASM({testSend("THIS IS A MESSAGE")});
}

void recvTurn(char *msg, int len)
{
	SDL_Log("Received %s", msg);
}
