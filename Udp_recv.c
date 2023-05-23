#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>

int main(int argc, char *argv[])
{
    // tao socket theo giao thuc UDP
    int recevier = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int sender = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    // Khai bao dia chi ben gui:
    struct sockaddr_in addr1;
    addr1.sin_family = AF_INET;
    addr1.sin_addr.s_addr = htonl(argv[1]);
    addr1.sin_port = htons(atoi(argv[2]));

    // khai bao dia chi ben nhan
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    
    bind(recevier,(struct sockaddr *)&addr,sizeof(addr));

    fd_set fdread, fdtest;
    FD_ZERO(&fdread);
    FD_SET(STDIN_FILENO,&fdread);
    FD_SET(recevier, &fdread);

    // Nhan tin nhan
    char buf[32];
    while (1)
    {
        int maxdp = recevier +1;
        select(recevier +1,&fdread,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO, &fdread)) // kiem tra ban phim
        {
            fgets(buf,sizeof(buf),stdin);
            sendto(sender,buf,strlen(buf), 0);
        }
        if(FD_ISSET(recevier,&fdread))
        {
            ret = recv(recevier,buf,sizeof(buf))
        }
       
    }
    
}