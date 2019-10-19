#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_STRING_SIZE 256

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

int main()
{
    char str[MAX_INPUT_STRING_SIZE];
    fgets(str, MAX_INPUT_STRING_SIZE, stdin);

    char* delimiters = " ,-\t\n";

    int tokensCount = 0;

    char** tokens = (char**)malloc(MAX_INPUT_STRING_SIZE * sizeof(char));

    Split(str, delimiters, &tokens, &tokensCount);

    printf("There are %d tokens\n", tokensCount);

    for(int i = 0; i < tokensCount; i++)
    {
        printf("%s\n", tokens[i]);
    }

    free(tokens);

    return 0;
}
