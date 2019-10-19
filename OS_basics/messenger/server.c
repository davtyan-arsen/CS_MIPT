#include "macro/server.h"

void *handler(void *arg)
{
    HandlerInfo info = *(HandlerInfo*)arg;
    int msqid = info.msqid;
    int *curNumberOfClients = &info.curNumberOfClients;
    InfoClient *clients = info.clients;
    int n;
    int msgLen;
    msgbuf handlerCommand;
    while(1)
    {
        if(msgLen = msgrcv(msqid, &handlerCommand, MAX_MSG_Q_LEN, 1, 0) < 0)
        {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        if(handlerCommand.command == SEND_TO_COMMAND)
        {
            int receiver = FindByName(clients, handlerCommand.receiver);
            int sender = FindClient(clients, &handlerCommand.addr);
            if(receiver >= 0 && sender >= 0)
            {
                char sendline[MAX_MESSAGE_LENGTH];
                bzero(sendline, MAX_MESSAGE_LENGTH);
                strcpy(sendline, "[");
                strcat(sendline, clients[sender].name);
                strcat(sendline, "] ");
                strcat(sendline, handlerCommand.message);

                if((n = write(clients[receiver].sockfd, sendline, sizeof(sendline) + 1)) < 0)
                {
                    perror(NULL); close(clients[receiver].sockfd); exit(-1);
                }
            }
        }
        else if(handlerCommand.command == SEND_NAME_LIST)
        {
            int sender = FindClient(clients, &handlerCommand.addr);
            if((sender >= 0) && clients[sender].isTaken == 1)
            {
                char sendline[MAX_MESSAGE_LENGTH];
                GetList(clients, sendline);
                printf("%s\n", sendline);
                if((n = write(clients[sender].sockfd, sendline, sizeof(sendline) + 1)) < 0)
                {
                    perror(NULL); close(clients[sender].sockfd); exit(-1);
                }
            }
            else
                printf("Incorrect ip\n");
        }
        else if(handlerCommand.command == FREE_CLIENT_COMMAND)
        {
            int i = FindClient(clients, &handlerCommand.addr);
            if(i >= 0)
            {
                close(clients[i].sockfd);
                clients[i].isTaken = 0;
                *curNumberOfClients -= 1;
            }
            else
                printf("Nothing to free\n");
        }
        else if(handlerCommand.command == SEND_ALL_COMMAND)
        {
            char sendline[MAX_MESSAGE_LENGTH];
            int sender = FindClient(clients, &handlerCommand.addr);
            bzero(sendline, MAX_MESSAGE_LENGTH);
            strcpy(sendline, "[");
            strcat(sendline, clients[sender].name);
            strcat(sendline, "] ");
            strcat(sendline, handlerCommand.message);

            for(int i = 0; i < MAX_CLIENT_NUMBER; i++)
            {
                if(clients[i].isTaken == 1)
                {
                    if((n = write(clients[i].sockfd, sendline, sizeof(sendline) + 1)) < 0)
                    {
                        perror(NULL); close(clients[i].sockfd); exit(-1);
                    }
                }
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    int msqid;
    char pathname[] = "server.c";
    key_t msqKey;

    if((msqKey = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate message queue key\n");
        exit(-1);
    }

    if((msqid = msgget(msqKey, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    InfoClient clients[MAX_CLIENT_NUMBER] = {0, 0, 0, 0};
    int curNumberOfClients = 0;

    int listenfd, addfd;
    int clilen;
    int n;
    char line[MAX_MESSAGE_LENGTH];
    struct sockaddr_in servaddr, cliaddr;

    MakeServerSocket(&listenfd, &servaddr, SERVER_PORT);

    while(1)
    {
        clilen = sizeof(cliaddr);

        if((addfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
        {
            perror(NULL); close(listenfd); exit(-1);
        }

        printf("%d %d\n", cliaddr.sin_addr.s_addr, cliaddr.sin_port);

        if(curNumberOfClients < MAX_CLIENT_NUMBER)
        {
            curNumberOfClients += 1;

            if((n = read(addfd, line, MAX_MESSAGE_LENGTH - 1)) < 0)
            {
                perror(NULL); close(listenfd); close(addfd); exit(-1);
            }

            printf("%s\n", line);

            int newClientIndex = AddNewClient(clients, &cliaddr, line);
            clients[newClientIndex].sockfd = addfd;

            pthread_t thid;
            HandlerInfo info;
            info.msqid = msqid;
            info.curNumberOfClients = curNumberOfClients;
            info.clients = clients;
            int result = pthread_create(&thid, NULL, handler, &info);

            pid_t singleClientListener = fork();
            if(singleClientListener == 0)
            {
                while((n = read(addfd, line, MAX_MESSAGE_LENGTH - 1)) > 0)
                {
                    printf("%s\n", line);
                    char tmp[MAX_MESSAGE_LENGTH];
                    strcpy(tmp, line);
                    char commandDelimiters[] = "**";
                    char** tokens = (char**)malloc(MAX_INPUT_STRING_SIZE * sizeof(char));
                    Split(tmp, commandDelimiters, &tokens);

                    if((strcmp(tokens[0], "list") == 0) || (strcmp(tokens[0], "LIST") == 0))
                    {
                        int msgLen;
                        msgbuf handlerCommand;
                        handlerCommand.mtype = 1;
                        handlerCommand.command = SEND_NAME_LIST;
                        handlerCommand.addr = cliaddr;
                        msgLen = sizeof(handlerCommand) - sizeof(long);
                        if(msgsnd(msqid, &handlerCommand, msgLen, 0) < 0)
                        {
                            printf("Can\'t send message to queue\n");
                            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                            exit(-1);
                        }
                    }
                    else if(((strcmp(tokens[0], "to") == 0) || (strcmp(tokens[0], "TO") == 0) || (strcmp(tokens[0], "To") == 0)) && (tokens[1] != NULL) && (tokens[2] != NULL))
                    {
                        int msgLen;
                        msgbuf handlerCommand;
                        handlerCommand.mtype = 1;
                        handlerCommand.command = SEND_TO_COMMAND;
                        strcpy(handlerCommand.receiver, tokens[1]);
                        strcpy(handlerCommand.message, tokens[2]);
                        handlerCommand.addr = cliaddr;
                        msgLen = sizeof(handlerCommand) - sizeof(long);
                        if(msgsnd(msqid, &handlerCommand, msgLen, 0) < 0)
                        {
                            printf("Can\'t send message to queue\n");
                            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                            exit(-1);
                        }
                    }
                    else
                    {
                        int msgLen;
                        msgbuf handlerCommand;
                        handlerCommand.mtype = 1;
                        handlerCommand.command = SEND_ALL_COMMAND;
                        strcpy(handlerCommand.message, line);
                        handlerCommand.addr = cliaddr;
                        msgLen = sizeof(handlerCommand) - sizeof(long);
                        if(msgsnd(msqid, &handlerCommand, msgLen, 0) < 0)
                        {
                            printf("Can\'t send message to queue\n");
                            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                            exit(-1);
                        }
                    }
                }

                if(n == 0)
                {
                    printf("%s disconnected\n", clients[newClientIndex].name);
                    int msgLen;
                    msgbuf handlerCommand;
                    handlerCommand.mtype = 1;
                    handlerCommand.command = FREE_CLIENT_COMMAND;
                    handlerCommand.addr = cliaddr;
                    msgLen = sizeof(handlerCommand) - sizeof(long);
                    if(msgsnd(msqid, &handlerCommand, msgLen, 0) < 0)
                    {
                        printf("Can\'t send message to queue\n");
                        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                        exit(-1);
                    }
                    break;
                }

                if(n < 0)
                {
                    perror(NULL); close(listenfd); close(addfd); exit(-1);
                }
            }
        }
        else
        {
            if((n = write(addfd, "Server is full", 15)) < 0)
            {
                perror(NULL); close(listenfd); close(addfd); exit(-1);
            }

            close(addfd);
        }
    }

    return 0;
}
