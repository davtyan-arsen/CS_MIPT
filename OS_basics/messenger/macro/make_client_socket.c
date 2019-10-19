#include "client.h"

void MakeClientSocket(int *sockfd, struct sockaddr_in *addr, int port, char *ip)
{
    if((*sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }

    bzero(addr, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    if(inet_aton(ip, &addr->sin_addr) == 0){
        printf("Invalid IP adress\n");
        close(*sockfd);
        exit(1);
    }

    if(connect(*sockfd, (struct sockaddr *) addr, sizeof(*addr)) < 0)
    {
        perror(NULL);
        close(*sockfd);
        exit(1);
    }
}
