#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE    1024
#define SMALL_BUF   100

void* request_handler(void *arg);
void send_data(FILE *fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);
void error_handling(char * msg);

int main(int argc, char * argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    char buf[BUF_SIZE];
    pthread_t t_id;
    int *temp_clnt_sock;

    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 创建服务器端套接字
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        error_handling("socket() error!");
    }

    // 地址信息初始化
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    
    // 分配地址信息
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
    {
        error_handling("bind() error!");
    }

    if(listen(serv_sock, 20) == -1) 
    {
        error_handling("listen() error!");
    }

    while(1) {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1) 
        {
            error_handling("accept() error");
        }
        printf("Connected Request : %s:%d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
        printf("Connected sock : %d\n", clnt_sock);
        temp_clnt_sock = (int *)malloc(sizeof(int));
        *temp_clnt_sock = clnt_sock;
        if(pthread_create(&t_id, NULL, request_handler, temp_clnt_sock) != 0) { // 这里传clnt_sock的地址是不是不太对? 变成了大家共用的, 可能会覆盖吧
            error_handling("pthread_create() error");
        }
        pthread_detach(t_id);
    }

    close(serv_sock);
    return 0;
}

void* request_handler(void *arg) {
    int clnt_sock = *((int *)arg);
    free(arg); // 之前malloc出来的, 用完free掉
    printf("request_handler, clnt_sock = %d\n", clnt_sock);
    char req_line[SMALL_BUF];
    FILE* clnt_read;
    FILE* clnt_write;

    char method[10];
    char ct[15];
    char file_name[30];

    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF, clnt_read);
    printf("req_line: %s\n", req_line);

    if(strstr(req_line, "HTTP/") == NULL) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));

    printf("method: %s; file_name = %s; ct = %s\n", method, file_name, ct);

    if(strcmp(method, "GET") != 0) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
    fclose(clnt_write);
    return NULL;
}

void send_error(FILE* fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                "<body><font size=+5><br>发生错误! 查看请求文件名和请求方式! "
                "</font></body></html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

char* content_type(char* file){
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if(!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/palin";
}

void send_data(FILE *fp, char* ct, char* file_name) {
    char protocol[] = "HTTP/1.1 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE* send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");
    if(send_file == NULL) {
        send_error(fp);
        return;
    }

    fputs(protocol, fp);
    printf("protocol: %s\n", protocol);
    fputs(server, fp);
    printf("server: %s\n", server);
    fputs(cnt_len, fp);
    printf("cnt_len: %s\n", cnt_len);
    fputs(cnt_type, fp);
    printf("cnt_type: %s\n", cnt_type);

    while(fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        printf("buf: %s\n", buf);
        fflush(fp);
    }
    fflush(fp);
}

void error_handling(char* msg) 
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

// gcc -o webserv_linux webserv_linux.c -D_REENTERANT -lpthread
// 有点问题, 浏览器没有正常显示数据, 后面看看