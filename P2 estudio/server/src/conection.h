#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "comunication.h"
#include "players.h"


struct lock
{
    int value;
};

struct args
{
    Player** players;
    int server_socket;
    socklen_t addr_pointer;
    struct lock* lock;
};

typedef struct args Args;
typedef struct lock Lock;
Player** prepare_sockets_and_get_clients(char * IP, int port, pthread_t* thread, Lock* lock);
int test_and_set(Lock* lock);
void acquire(Lock* lock);
void release(Lock* lock);