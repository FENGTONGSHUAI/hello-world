#include <stdio.h>
#include <stdlib.h>

long unsigned int maximum = 0;

int main(int argc, char *argv[])
{
    long unsigned int blocksize[] = {1024 * 1024 * 1024, 1024 * 1024, 1024, 1};
    int i, count;
    long unsigned int tempMaxinum;
    for(i = 0; i < 4; i++) {
        printf("before break, i = %d\n", i);
        tempMaxinum = maximum;
        for(count = 1;; count++) {
            void *block = malloc(tempMaxinum + blocksize[i] * count);
            if(block) {
                maximum = tempMaxinum + blocksize[i] * count;
                printf("maximum = %ld, tempMaxinum = %ld, i = %d, count = %d\n", maximum, tempMaxinum, 
                    i, count);
                free(block);
            } else {
                break;
            }
        }
        printf("after break, i = %d\n", i);
    }
    printf("maximum malloc size = %lu bytes\n", maximum);
    return 0;
}

/*
gcc testMalloc.c -o testMalloc
wsl测试, 我的电脑内存16G, 测试出来结果是14517097472, 14.5G, 是空闲物理内存+空闲swap之和
服务器测试, 服务器内存是1.7G左右, 测试出来结果是1805236536, 1.8G, 查看swap是0, 不知道为啥
*/