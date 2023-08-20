#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char* argv[])
{
    int acpt_sock, recv_sock;
    char buf[BUF_SIZE];
    int str_len, state;

    struct sockaddr_in acpt_adr, recv_adr;
    socklen_t recv_adr_sz;

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建服务器端套接字
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(acpt_sock == -1) {
        error_handling("socket() error!");
    }

    // 地址信息初始化
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(atoi(argv[1]));
    
    // 分配地址信息
    if(bind(acpt_sock, (struct sockaddr*) &acpt_adr, sizeof(acpt_adr)) == -1)
    {
        error_handling("bind() error!");
    }

    if(listen(acpt_sock, 5) == -1) 
    {
        error_handling("listen() error!");
    }

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);

    if(recv_sock == -1) 
    {
        error_handling("accept() error");
    }

    while(1) 
    {
        str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK|MSG_DONTWAIT);
        if(str_len > 0)
            break;
    }

    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);

    str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
    buf[str_len] = 0;
    printf("Read again: %s \n", buf);

    close(recv_sock);
    close(acpt_sock);
    return 0;
}

void error_handling(char* message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// gcc -o peek_recv peek_recv.c