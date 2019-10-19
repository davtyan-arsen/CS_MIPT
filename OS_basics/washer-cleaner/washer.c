#include "split/split.h"
#include "macro/macro.h"

#define NAME_INDEX 0
#define WASHING_TIME_INDEX 1
#define CLEANING_TIME_INDEX 2

int main(int argc, char const *argv[])
{
    int msqid;
    char pathname[] = "table.c";
    key_t msqKey;
    int msgLen;

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

    FILE* file = fopen("dishes.txt", "r");

    char str[MAX_INPUT_STRING_SIZE];
    fgets(str, MAX_INPUT_STRING_SIZE, file);

    int dishesCount = atoi(str);
    char* delimiters = " \t\n";

    for (int i = 0; i < dishesCount; i++)
    {
        char** tokens = (char**)malloc(MAX_INPUT_STRING_SIZE * sizeof(char));
        fgets(str, MAX_INPUT_STRING_SIZE, file);
        Split(str, delimiters, &tokens);

        msgbuf dishInfo;
        dishInfo.mtype = WT_MSG_TYPE;
        strcpy(dishInfo.name, tokens[NAME_INDEX]);
        dishInfo.washingTime = atoi(tokens[WASHING_TIME_INDEX]);
        dishInfo.cleaningTime = atoi(tokens[CLEANING_TIME_INDEX]);

        msgLen = sizeof(dishInfo) - sizeof(long);

        printf("[_WASHER_]: Start washing %s (%d %d)\n", dishInfo.name, dishInfo.washingTime, dishInfo.cleaningTime);
        sleep(dishInfo.washingTime);

        ExecuteSemAction(&semid, 0, -1);

        if(msgsnd(msqid, &dishInfo, msgLen, 0) < 0)
        {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }

        printf("[_WASHER_]: Finish washing %s (%d %d)\n", dishInfo.name, dishInfo.washingTime, dishInfo.cleaningTime);
    }

    printf("[_WASHER_]: No more dishes to wash\n");
    finishBuf finish;
    finish.mtype = W_FINISH_SIGNAL;
    msgLen = sizeof(finish) - sizeof(long);
    if(msgsnd(msqid, &finish, msgLen, 0) < 0)
    {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    return 0;
}
