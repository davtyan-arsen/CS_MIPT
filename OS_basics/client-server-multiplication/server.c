#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MSG_LEN 1000000
#define SERVER_ID 1
#define MAX_NUMBER_OF_CLIENTS 5
#define VIRTUAL_TIME_DELAY 10

struct RequestBuf
{
    long mtype;
    int pid;
    int a;
    int b;
};

struct ResponseBuf
{
    long mtype;
    int result;
};

int ExecuteSemAction(int* semid, int semNumber, int semAction)
{
    struct sembuf semBuf;

    semBuf.sem_op = semAction;
    semBuf.sem_flg = 0;
    semBuf.sem_num = semNumber;

    return semop(*semid, &semBuf, 1);
}

int main(int argc, char const *argv[])
{
    int msqid;
    char pathname[] = "server.c";
    key_t msqKey;
    int msgLen;

    pid_t pid;

    struct RequestBuf request;
    struct ResponseBuf response;

    if((msqKey = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate message queue key\n");
        exit(-1);
    }

    if((msqid = msgget(msqKey, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    int semid;
    key_t semKey;

    if((semKey = ftok(pathname, 0)) < 0){
        printf("Can\'t generate semaphore key\n");
        exit(-1);
    }

    if((semid = semget(semKey, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }

    if(ExecuteSemAction(&semid, 0, MAX_NUMBER_OF_CLIENTS) < 0){
        printf("Can\'t set max number of clients\n");
        exit(-1);
    }

    while(1)
    {
        if(msgLen = msgrcv(msqid, &request, MAX_MSG_LEN, SERVER_ID, 0) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        pid = fork();

        if(pid == 0)
        {
            printf("New client request, pid: %d\n", request.pid);
            ExecuteSemAction(&semid, 0, -1);

            response.mtype = request.pid;
            response.result = request.a * request.b;

            msgLen = sizeof(response) - sizeof(long);

            sleep(VIRTUAL_TIME_DELAY);

            if(msgsnd(msqid, &response, msgLen, 0) < 0)
            {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }

            printf("Finished working with client, pid: %d\n", request.pid);
            ExecuteSemAction(&semid, 0, 1);
        }
    }

    return 0;
}
