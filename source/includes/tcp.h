#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#ifdef _WIN32
 #include <WinSock2.h>
 #define _WINSOCK_DEPRECATED_NO_WARNINGS
 #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #define SOCKET_ERROR -1
    #define SOCKET int
#endif

#include "debug.h"


#define VALIDATE(_return, success, err) { \
    if (_return < 0) {   \
        DEBUG(err, errno); \
        cleanup(sock);\
        }\
    else                \
        DEBUG(success);      \
        }

# define IP "127.0.0.1"
# define PORT 8080
# define BUFF_SIZE 1024

void cleanup(int sock) {
    printf("Cleaning up...");
    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif
    exit(0);
}
