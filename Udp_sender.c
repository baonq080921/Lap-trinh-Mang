#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include<sys/select.h>
#include <arpa/inet.h>
int main(int argc, char *argv[])
{
    // Khai bao socket:
    int sender = socket(AF_INET, SOCK_STREAM, IPPROTO_UDP);
    if (sender == -1)
    {
        perror("connect failed()");
        return 1;
    }
    // Khai bao dia chi ben nhan
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = inet_addr(argv[1]);
    addr1.sin_port = htons(atoi(argv[2]));

    //Khoi tao select:
    fd_set fdread;
    FD_ZERO(&fdread);
    FD_SET(rec,)
    while ((1))
    {
        FD_SET

    }

    return 0;
}