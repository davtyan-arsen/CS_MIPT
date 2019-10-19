#include "macro.h"

int ExecuteSemAction(int* semid, int semNumber, int semAction)
{
    struct sembuf semBuf;

    semBuf.sem_op = semAction;
    semBuf.sem_flg = 0;
    semBuf.sem_num = semNumber;

    return semop(*semid, &semBuf, 1);
}
