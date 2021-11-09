#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "comunication.h"
#include "players.h"

struct args
{
    Player** players;
    int server_socket;
    socklen_t* addr_pointer;
};

typedef struct args Args;
Player** prepare_sockets_and_get_clients(char * IP, int port);
