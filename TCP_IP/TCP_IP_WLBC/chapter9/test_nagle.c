#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/tcp.h>
#include <netinet/in.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
    int sock;
    int opt_val;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);

    len = sizeof(socklen_t);
    state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&opt_val, &len);
    if(state)
        error_handling("getsockopt() error!");
    
    printf("Before set TCP_NODELAY: %d \n", opt_val);

    opt_val = 1;
    state = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&opt_val, len);
    if(state)
        error_handling("setsockopt() error!");

    state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void *)&opt_val, &len);
    if(state)
        error_handling("getsockopt() error!");
    
    printf("After set TCP_NODELAY: %d \n", opt_val);

    return 0;
}

void error_handling(char* message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// gcc -o test_nagle test_nagle.c