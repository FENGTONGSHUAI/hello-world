#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUF_SIZE 3

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int fd1, fd2;
    int file_size;
    char buf[BUF_SIZE];

    if(argc != 2) {
        printf("Usage : %s <filesize>\n", argv[0]);
        exit(1);
    }

    file_size = atoi(argv[1]);
    
    if(file_size <= 0) {
        error_handling("file_size must bigger than 0");
    }

    // 以写文本的方式向文件中写出数据
    FILE *p = fopen("news.txt", "w");

    // 将指针指向 距离开始位置 file_size - 1 字节偏移位置
    // 这里特别注意 , 要预留出一个字节 , 用于做数据填充
    fseek(p, file_size - 1, SEEK_SET);

    // 在文件内部指针指向的第 file_size 字节处 , 写入一个字节数据
    char a = 0;
    fwrite(&a, 1, sizeof(a), p);

    // 关闭文件
    fclose(p);

    return 0;
}

void error_handling(char* message) 
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// gcc -o gen_test_file gen_test_file.c