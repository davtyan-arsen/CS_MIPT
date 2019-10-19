#include "macro/client.h"

int main(int argc, char const *argv[])
{
    int sockfd;
    int n;
    char line[MAX_MESSAGE_LENGTH];
    struct sockaddr_in servaddr;

    char ip[MAX_IP_LENGTH];
    strcpy(ip, argv[1]);

    char name[MAX_NAME_LENGTH];
    strcpy(name, argv[2]);

    if(argc != 3)
    {
        printf("Usage: ./client <IP address> NAME\n");
        exit(-1);
    }

    bzero(line, MAX_MESSAGE_LENGTH);

    MakeClientSocket(&sockfd, &servaddr, SERVER_PORT, ip);

    if((n = write(sockfd, name, strlen(name) + 1)) < 0)
    {
        perror("Cant write\n");
        close(sockfd);
        exit(-1);
    }

    int pid = fork();
    if(pid == 0)
    {
        fflush(stdin);

        while(1)
        {
            fgets(line, MAX_MESSAGE_LENGTH, stdin);
            if((n = write(sockfd, line, strlen(line) + 1)) < 0)
            {
                perror("Cant write\n");
                close(sockfd);
                exit(-1);
            }
        }
    }
    else
    {
        while(1)
        {
            if((n = read(sockfd, line, MAX_MESSAGE_LENGTH - 1)) > 0)
            {
                printf("%s\n", line);
            }
        }
    }

    close(sockfd);

    return 0;
}
