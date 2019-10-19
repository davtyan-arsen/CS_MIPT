#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 30

#define START_DIR_INDEX 1
#define SEARCH_DEPTH_INDEX 2
#define TARGET_FILE_INDEX 3

int Search(char* startDir, int searchDepth, char* targetFile, char* resultPath)
{
    if(searchDepth >= 0)
    {
        DIR* dir;
        struct dirent* dirEntry;
        dir = opendir(startDir);
        if(dir == NULL)
        {
            printf("Cannot open the directory\n");
            exit(-1);
        }

        while( (dirEntry = readdir(dir)) != NULL)
        {
            char currentPath[MAX_PATH_LENGTH] = "";
            strcat(currentPath, startDir);
            strcat(currentPath, "/");
            strcat(currentPath, dirEntry->d_name);

            struct stat statbuf;
            int statResult = stat(currentPath, &statbuf);

            if( (strcmp(targetFile, dirEntry->d_name) == 0) && S_ISREG(statbuf.st_mode))
            {
                strcpy(resultPath, currentPath);
                return 1;
            }
            else if( S_ISDIR(statbuf.st_mode) && strcmp(dirEntry->d_name, ".") && (strcmp(dirEntry->d_name, "..")) )
            {
                Search(currentPath, searchDepth - 1, targetFile, resultPath);
            }
        };
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    char startDir[MAX_PATH_LENGTH] = "";
    strcpy(startDir, argv[START_DIR_INDEX]);

    int searchDepth = atoi(argv[SEARCH_DEPTH_INDEX]);

    char targetFile[MAX_PATH_LENGTH] = "";
    strcpy(targetFile, argv[TARGET_FILE_INDEX]);

    char resultPath[MAX_PATH_LENGTH] = "";

    Search(startDir, searchDepth, targetFile, resultPath);
    printf("%s\n", resultPath);

    return 0;
}
