#include <string.h>
#include "split.h"

void Split(char* string, char* delimiters, char*** tokens)
{
    int tokensCount = 0;

    char** tokensArray = *tokens;
    char* token = strtok(string, delimiters);

    while(token != NULL)
    {
        tokensArray[tokensCount] = token;
        ++tokensCount;

        token = strtok(NULL, delimiters);
    }
}
