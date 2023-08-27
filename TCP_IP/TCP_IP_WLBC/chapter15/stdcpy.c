#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define BUF_SIZE 3

int main(int argc, char *argv[])
{
    FILE *fp1;
    FILE *fp2;
    int len;
    char buf[BUF_SIZE];
    clock_t start, finish;
	double total_time;

    fp1 = fopen("news.txt", "r");
    fp2 = fopen("cpy.txt", "w");

    start = clock();
    while(fgets(buf, BUF_SIZE, fp1) != NULL) {
        fputs(buf, fp2);
    }
    finish = clock();
    total_time = (double)(finish - start) / CLOCKS_PER_SEC; //单位换算成秒
    printf("%f seconds \n", total_time);

    fclose(fp1);
    fclose(fp2);
    return 0;
}

// gcc -o stdcpy stdcpy.c