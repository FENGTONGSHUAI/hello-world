#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define BUF_SIZE 3

int main(int argc, char *argv[])
{
    int fd1, fd2;
    int len;
    char buf[BUF_SIZE];
    clock_t start, finish;
	double total_time;

    fd1 = open("news.txt", O_RDONLY);
    fd2 = open("cpy.txt", O_WRONLY|O_CREAT|O_TRUNC);

    start = clock();
    while((len = read(fd1, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }
    finish = clock();
    total_time = (double)(finish - start) / CLOCKS_PER_SEC; //单位换算成秒
    printf("%f seconds \n", total_time);

    close(fd1);
    close(fd2);
    return 0;
}

// gcc -o syscpy syscpy.c