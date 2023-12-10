#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define KB_SIZE 1024
#define MB_SIZE KB_SIZE * 1024
#define GB_SIZE  MB_SIZE * 1024
#define MEM_SIZE MB_SIZE * 128  // 每次分配128M
#define MALLOC_TIME 8           // 分配8次, 共1G

int main(void) {
    pid_t pid;
    char* addr[MALLOC_TIME];
    int i = 0;

    pid = getpid();
    printf("test_virt, pid = %d\n", pid);
    sleep(5);  // 等待5秒
    for(i = 0; i < MALLOC_TIME; ++i) {
        addr[i] = (char*) malloc(MEM_SIZE);
        if(!addr[i]) {
            printf("执行 malloc 失败, 错误：%s\n", strerror(errno));
		        return -1;
        }
        printf("%d: 主线程调用malloc后，申请128MB大小得内存，此内存起始地址：0X%p\n", i, addr[i]);
        sleep(2);  // 等待两秒
    }

    for(i = 0; i < MALLOC_TIME; ++i) {
        free(addr[i]);
        printf("%d: 主线程调用free后，申请释放128MB大小得内存，此内存起始地址：0X%p\n", i, addr[i]);
        sleep(2);  // 等待两秒
    }

    getchar();
    return 0
}

// gcc -o test_virt test_virt.c  && ./test_virt
// top -p <pid>
// export LD_PRELOAD=/usr/local/lib/libjemalloc.so
// unset LD_PRELOAD
// lsof -n | grep <pid> | grep jemalloc