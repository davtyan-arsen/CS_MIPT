#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_STRING_SIZE 64
#define TIMEOUT 5

#define DELAY_TOKEN_INDEX 0
#define COMMAND_NAME_TOKEN_INDEX 1

void Split(char* string, char* delimiters, char*** tokens, int* tokensCount)
{
    *tokensCount = 0;

    char** tokensArray = *tokens;
    char* token = strtok(string, delimiters);

    while(token != NULL)
    {
        tokensArray[*tokensCount] = token;
        ++*tokensCount;

        token = strtok(NULL, delimiters);
    }
}

int main(int argc, char const *argv[])
{
    FILE* file = fopen("file.txt", "r");

    char str[MAX_INPUT_STRING_SIZE];
    fgets(str, MAX_INPUT_STRING_SIZE, file);

    int taskCount = atoi(str);

    int processPids[taskCount];
    int status;

    char* delimiters = " \t\n";

    for (int i = 0; i < taskCount; i++)
    {
        int tokensCount;
        char** tokens = (char**)malloc(MAX_INPUT_STRING_SIZE * sizeof(char));

        fgets(str, MAX_INPUT_STRING_SIZE, file);

        Split(str, delimiters, &tokens, &tokensCount);

        int delay = atoi(tokens[DELAY_TOKEN_INDEX]);

        processPids[i] = fork();

        if(processPids[i] == 0)
        {
            pid_t timerPid = fork();

            if(timerPid == 0)
            {
                sleep(delay);
                execvp(tokens[COMMAND_NAME_TOKEN_INDEX], &tokens[COMMAND_NAME_TOKEN_INDEX]);
            }
            else if(timerPid > 0)
            {
                sleep(delay + TIMEOUT);

                if(waitpid(timerPid, &status, WNOHANG))
                    exit(0);

                else
                {
                    kill(timerPid, SIGKILL);
                    printf("process killed\n");
                    exit(0);
                }
            }
        }
    }

    fclose(file);
    return 0;
}
