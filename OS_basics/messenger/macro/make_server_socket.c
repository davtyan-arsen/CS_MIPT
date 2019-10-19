#include "server.h"

void MakeServerSocket(int *sockfd, struct sockaddr_in *addr, int port)
{
    if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }

    bzero(addr, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(*sockfd, (struct sockaddr *) addr, sizeof(*addr)) < 0)
    {
        perror(NULL);
        close(*sockfd);
        exit(1);
    }

    if(listen(*sockfd, MAX_CLIENT_NUMBER) < 0)
    {
        perror(NULL);
        close(*sockfd);
        exit(1);
    }
}
