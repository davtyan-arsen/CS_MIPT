#include "server.h"

int FindClient(InfoClient *clients, struct sockaddr_in *addr)
{
    for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if((clients[i].isTaken == 1) && (clients[i].addr.sin_addr.s_addr == addr->sin_addr.s_addr) && (clients[i].addr.sin_port == addr->sin_port))
            return i;
    }

    return -1;
}

int FindByName(InfoClient *clients, char *name)
{
    for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if((clients[i].isTaken == 1) && (strcmp(clients[i].name, name) == 0))
            return i;
    }

    return -1;
}

int AddNewClient(InfoClient *clients, struct sockaddr_in *addr, char *name)
{
    for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if(clients[i].isTaken == 0)
        {
            clients[i].addr = *addr;
            strcpy(clients[i].name, name);
            clients[i].isTaken = 1;

            return i;
        }
    }

    return -1;
}

void GetList(InfoClient *clients, char *sendline)
{
    bzero(sendline, MAX_MESSAGE_LENGTH);
    for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
    {
        if(clients[i].isTaken == 1)
        {
            strcat(sendline, "<");
            printf("%d ", i);
            strcat(sendline, clients[i].name);
            strcat(sendline, "> ");
        }
    }
}
