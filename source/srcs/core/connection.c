#include "../includes/game.h"
#include "../includes/tcp.h"

#include <errno.h>
#include <fcntl.h>

static struct sockaddr_in  addr;
static SOCKET sock;
// char buffer[BUFF_SIZE];

void recvMessage(Context *ctx)
{
    memset(ctx->connection.message, '\0', sizeof(ctx->connection.message));
    while (recv(sock, ctx->connection.message, sizeof(ctx->connection.message), 0))
    {
        ctx->connection.hasMessage = SDL_TRUE;
    }
}

void initConnection(Context *ctx)
{
    #ifdef _WIN32
        WSADATA wsdata;
        if ( WSAStartup(MAKEWORD(2,2), &wsdata)== SOCKET_ERROR){
            printf ("Error initialising WSA.\n");
            return -1;
        }
    #else
        int sock;
    #endif

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = PORT;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

//TODO: CROSS PLATFORM NONBLOCK
    VALIDATE((sock = socket(AF_INET, SOCK_STREAM, O_NONBLOCK)), "Socket created", "Failed to create socket %d")
    // VALIDATE(connect(sock, (struct sockaddr*)&addr, sizeof(addr)), "Connected", "Failed to connect %d")
    int ret = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        #ifdef _WIN32
        int err = WSAGetLastError();
        #else
        int err = errno;
        #endif
        printf("Couldnt connect  errno %d\n", err);
        cleanup(sock);
    }
    ctx->connection.status = CONNECTED;
    // #ifdef _WIN32
    //     closesocket(sock);
    //     WSACleanup();
    // #else
    //     close(sock);
    // #endif

    return 0;
}

void closeConnection()
{
    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif
}

void sendMessage(char *message)
{
	// SDL_Log("Sending message %s", message);
	// EM_ASM(sendToConsole("test"));

	// EM_ASM({sendToConsole($0)}, message);
}
