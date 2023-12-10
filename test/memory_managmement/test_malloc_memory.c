/*
参考: https://www.xiaolincoding.com/os/3_memory/alloc_mem.html#_64-%E4%BD%8D%E7%B3%BB%E7%BB%9F%E7%9A%84%E5%9C%BA%E6%99%AF
自己测试一下
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MEM_SIZE 1024 * 1024 * 1024
#define MALLOC_TIME 1000000

int main() {
    char* addr[MALLOC_TIME];
    int i = 0;
    for(i = 0; i < MALLOC_TIME; ++i) {
        addr[i] = (char*) malloc(MEM_SIZE);
        if(!addr[i]) {
            printf("执行 malloc 失败, 错误：%s\n", strerror(errno));
		        return -1;
        }
        printf("%d: 主线程调用malloc后，申请1gb大小得内存，此内存起始地址：0X%p\n", i, addr[i]);
    }
    
    //输入任意字符后，才结束
    getchar();
    return 0;
}

// gcc -o test_malloc_memory test_malloc_memory.c