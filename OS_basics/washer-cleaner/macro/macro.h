#ifndef MACRO_H
#define MACRO_H

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define WT_MSG_TYPE 1 // Washer -> Table
#define TC_MSG_TYPE 2 // Table -> Cleaner
#define W_FINISH_SIGNAL 3 // Washer finished its work: no new dishes for table
#define T_FINISH_SIGNAL 4 // Table finished its work: no new dishes for cleaner

#define TABLE_CAPACITY 1

#define MAX_MSG_LEN 1024
#define MAX_NAME_LENGTH 10

typedef struct msgbuf {
    long mtype;

    char name[MAX_NAME_LENGTH];
    int washingTime;
    int cleaningTime;
} msgbuf;

typedef struct finishBuf {
    long mtype;
} finishBuf;

int ExecuteSemAction(int*, int, int);

#endif
