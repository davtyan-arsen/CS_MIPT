#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_STRING_SIZE 100

#define USER_1_ID 0
#define USER_2_ID 1

void openFD(int userId, int* fd, int oflag)
{
    int fdIndex;
    oflag == O_WRONLY ? (fdIndex = 0) : (fdIndex = 1);

    ((userId + fdIndex) % 2 ) == 0 ? (fd[fdIndex] = open("fifo1", oflag) )
                                   : (fd[fdIndex] = open("fifo2", oflag) );
}

void UserAction(int userId, int pid, int* fd, char* stringInput, char* stringOutput)
{
    pid == 0 ? openFD(userId, fd, O_WRONLY) : openFD(userId, fd, O_RDONLY);

    while(1)
    {
        if(pid == 0)
        {
            fgets(stringInput, MAX_STRING_SIZE, stdin);
            write(fd[0], stringInput, MAX_STRING_SIZE);
        }
        else
        {
            read(fd[1], stringOutput, MAX_STRING_SIZE);
            printf("%s", stringOutput);
        }
    }
}

int main(int argc, char const *argv[])
{
    int userId = atoi(argv[1]);

    int fd[2];

    char stringInput[MAX_STRING_SIZE];
    char stringOutput[MAX_STRING_SIZE];

    pid_t pid = fork();

    mkfifo("fifo1", 0777);
    mkfifo("fifo2", 0777);

    userId == USER_1_ID ? UserAction(USER_1_ID, pid, fd, stringInput, stringOutput)
                        : UserAction(USER_2_ID, pid, fd, stringInput, stringOutput);

    return 0;
}
