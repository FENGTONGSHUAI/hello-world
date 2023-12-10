#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KB_SIZE 1024
#define MB_SIZE KB_SIZE * 1024
#define GB_SIZE  MB_SIZE * 1024
#define MEM_SIZE (long long)16 * GB_SIZE

int main() {
    char* addr = (char*) malloc((long long)MEM_SIZE);
    printf("主线程调用malloc后，目前共申请了 %lldMB 的虚拟内存\n", MEM_SIZE / (1024 * 1024));
    
    //循环频繁访问虚拟内存
    while(1) {
          printf("开始访问 %lldMB 大小的虚拟内存...\n", MEM_SIZE / (1024 * 1024));
          memset(addr, 0, (long long)MEM_SIZE);
    }
    return 0;
}

// gcc -o test_oom test_oom.c && ./test_oom
// 显示错误为   Segmentation fault (core dumped)