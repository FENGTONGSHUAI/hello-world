#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int recv_sock;
    char buf[BUF_SIZE];
    int str_len;
    struct sockaddr_in adr;
    if(argc != 2) {
        printf("Usage : %s <PORT>\n", argv[0]);
        exit(1);
    }

    // 新建套接字
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(recv_sock == -1) {
        error_handling("UDP scoket creation error");
    }

    // 地址信息初始化
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));
    
    // 分配地址信息
    if(bind(recv_sock, (struct sockaddr*) &adr, sizeof(adr)) == -1)
    {
        error_handling("bind() error!");
    }

    while(1) {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if(str_len < 0)
            break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }

    close(recv_sock);
    return 0;
}

void error_handling(char* message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// gcc -o news_receiver_brd news_receiver_brd.c