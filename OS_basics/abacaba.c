#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AppendCharToString(char* string, char ch)
{
    size_t len = strlen(string);
    string[len] = ch;
    string[len + 1] = '\0';
}

void GenerateString(int n, char* string)
{
    strcpy(string, "a");

    for(int i = 1; i < n; i++)
    {
        AppendCharToString(string, 'a' + i);
        strncat(string, string, (1 << i) - 1);
    }
};

int main()
{
    int n;
    scanf("%d", &n);

    char* str = (char*)malloc((1 << n) * sizeof(char));

    GenerateString(n, str);
    printf("%s", str);

    free(str);

    return 0;
}
