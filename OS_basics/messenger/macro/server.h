#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "../split/split.h"

#define MAX_MSG_Q_LEN 10000

#define SERVER_PORT 51000
#define MAX_CLIENT_NUMBER 15

#define MAX_MESSAGE_LENGTH 100
#define MAX_NAME_LENGTH 10

#define MAX_COMMAND_LENGTH 5

#define SEND_ALL_COMMAND 0
#define SEND_TO_COMMAND 1
#define FREE_CLIENT_COMMAND 2
#define SEND_NAME_LIST 3

typedef struct InfoClient {
    int isTaken;
    int sockfd;
    struct sockaddr_in addr;
    char name[MAX_NAME_LENGTH];
} InfoClient;

typedef struct HandlerInfo {
    int msqid;
    int curNumberOfClients;
    InfoClient *clients;
} HandlerInfo;

typedef struct msgbuf {
    long mtype;

    int command;

    struct sockaddr_in addr;
    char receiver[MAX_NAME_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
} msgbuf;

void MakeServerSocket(int*, struct sockaddr_in*, int);

int FindClient(InfoClient*, struct sockaddr_in*);
int FindByName(InfoClient*, char*);
int AddNewClient(InfoClient*, struct sockaddr_in*, char*);
void GetList(InfoClient*, char*);

#endif
