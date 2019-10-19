#include "macro/macro.h"

int main(int argc, char const *argv[])
{
    int msqid;
    char pathname[] = "table.c";
    key_t msqKey;
    int msgLen;

    msgbuf dishInfo;
    finishBuf finish;

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

    if((semid = semget(semKey, 2, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }

    ExecuteSemAction(&semid, 0, TABLE_CAPACITY);
    ExecuteSemAction(&semid, 1, 1);

    pid_t pid = fork();

    if(pid == 0)
    {
        while(1)
        {
            if(msgLen = msgrcv(msqid, &dishInfo, MAX_MSG_LEN, WT_MSG_TYPE, 0) < 0) {
                printf("Can\'t receive message from queue\n");
                exit(-1);
            }

            printf("got new plate\n");

            dishInfo.mtype = TC_MSG_TYPE;
            msgLen = sizeof(dishInfo) - sizeof(long);

            ExecuteSemAction(&semid, 1, -1);

            if(msgsnd(msqid, &dishInfo, msgLen, 0) < 0)
            {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }

            ExecuteSemAction(&semid, 0, 1);
        }
    }
    else
    {
        if(msgLen = msgrcv(msqid, &finish, MAX_MSG_LEN, W_FINISH_SIGNAL, 0) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        ExecuteSemAction(&semid, 0, -TABLE_CAPACITY);
        finish.mtype = T_FINISH_SIGNAL;

        if(msgsnd(msqid, &finish, msgLen, 0) < 0)
        {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

        kill(pid, SIGKILL);
    }

    return 0;
}
