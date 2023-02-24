#include "../includes/splendor.h"
#include <emscripten.h>
static Context *ctx;

void recvMessage(char *msg)
{
	SDL_Log("received message %s LEN -> %d", msg, MSG_LEN);
	ctx->connection.hasMessage = SDL_TRUE;
	SDL_memcpy(ctx->connection.message, msg, MSG_LEN);
	SDL_free(msg);
	// ctx->connection.message = msg;
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
	// sendMessage("Test");
}

void sendMessage(char *message)
{
	SDL_Log("Sending message %s", message);
	// EM_ASM(sendToConsole("test"));

	EM_ASM({sendToConsole($0)}, message);
}
