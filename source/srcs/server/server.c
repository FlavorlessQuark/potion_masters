#include "tcp.h"

#define MAX_OPEN_SOCK 5

typedef struct cli_data {
    pthread_mutex_t *mut;
    int index;
    pthread_attr_t attr;
    struct sockaddr_in *addr_data;
    pthread_t thread;
    // int *socks[MAX_OPEN_SOCK];
    int sock;
}       cli_data;

void *do_client(void *args)
{
    cli_data *data;
    char buffer[BUFF_SIZE];
    int sock;
    int ret;

    data = (cli_data *)args;
    // sock = data->socks[data->index];
    while (1)
    {
        memset(buffer, '\0', sizeof(buffer));
        ret = recv(data->sock, buffer, sizeof(buffer), 0);
        if (ret > 0)
        {
            printf("Client: %d -> %s\n",data->sock, buffer);
        }
    }

}

int main(){

    int sock, cli_sock;
    struct sockaddr_in serv_addr, cli_addr;
    int addr_size;
    pthread_mutex_t mut;
    cli_data clidata[MAX_OPEN_SOCK];
    int n;

    #ifdef _WIN32
    	WSDATA wsdata;
        WSAStartup(MAKEWORD(2,2), &wsdata);
    #endif

    memset(&serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = PORT;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    VALIDATE((sock = socket(AF_INET, SOCK_STREAM, 0)), "Socket created", "Failed to create socket")
    // n = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    VALIDATE((n = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))), "Bind success", "Bind fail");
    listen(sock, 5);
    printf("Listening...\n");

    n = 0;
    while(1){
        addr_size = sizeof(cli_addr);
        cli_sock = accept(sock, (struct sockaddr*)&cli_addr, &addr_size);
        if (cli_sock != -1)
        {
            printf("Client connected FD %d.\n", cli_sock);
            memset(&clidata[n], 0, sizeof(clidata[n]));
            clidata[n].sock = cli_sock;
            pthread_attr_init(&clidata[n].attr);
            pthread_create(&clidata[n].thread, &clidata[n].attr, do_client, &clidata[n]);
            n++;
        }

        // memset(buffer, '\0', sizeof(buffer));
        // recv(cli_sock, buffer, sizeof(buffer), 0);
        // printf("Client: %s\n", buffer);

        // memset(buffer, '\0', sizeof(buffer));
        // strcpy(buffer, "HI from server :0");
        // send(cli_sock, buffer, strlen(buffer), 0);

        //cleanup(cli_sock);

    }
    return 0;
}
