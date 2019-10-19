#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MSG_LEN 1000000
#define SERVER_ID 1

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

int main(int argc, char const *argv[])
{
    int msqid;
    char pathname[] = "server.c";
    key_t msqKey;
    int msgLen;

    pid_t pid = getpid();

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

    request.mtype = SERVER_ID;
    request.pid = pid;
    scanf("%d", &request.a);
    scanf("%d", &request.b);

    msgLen = sizeof(request) - sizeof(long);

    if(msgsnd(msqid, &request, msgLen, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    if(msgLen = msgrcv(msqid, &response, MAX_MSG_LEN, pid, 0) < 0) {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }

    printf("%d\n", response.result);

    return 0;
}
