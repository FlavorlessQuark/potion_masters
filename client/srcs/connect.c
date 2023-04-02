#include "../includes/splendor.h"
#include <emscripten.h>

static Context *ctx;
static char message[MSG_LEN];

void recvMessage(char *msg)
{
	SDL_Log("received message %s LEN -> %d", msg, MSG_LEN);
	SDL_memcpy(message, msg, MSG_LEN);
	SDL_free(msg);
	parseMsg(ctx, message);
}

void setConnectionStatus(int status)
{
	ctx->connection.status = status;
	SDL_Log("Set status to %d", status);
}

void initConnection(Context *context)
{
	ctx = context;
	context->state = CONNECT;
	EM_ASM({start_connect()});
	// ctx->connection.status == CONNECTED;
	// sendMessage("Test");
}

void sendMessage(char *message)
{
	SDL_Log("Sending message %s", message);
	// EM_ASM(sendToConsole("test"));

	EM_ASM({sendToConsole($0)}, message);
}
