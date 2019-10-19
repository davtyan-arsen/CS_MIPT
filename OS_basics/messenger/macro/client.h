#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define SERVER_PORT 51000

#define MAX_IP_LENGTH 16
#define MAX_MESSAGE_LENGTH 100
#define MAX_NAME_LENGTH 10

void MakeClientSocket(int*, struct sockaddr_in*, int, char*);

#endif
