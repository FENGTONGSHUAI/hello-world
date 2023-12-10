#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h>

#define KB_SIZE 1024
#define MB_SIZE KB_SIZE * 1024
#define GB_SIZE  MB_SIZE * 1024
#define MEM_SIZE MB_SIZE * 128  // 每次分配128M
#define MALLOC_TIME 8           // 分配8次, 共1G

// 参考 https://blog.csdn.net/hongge_smile/article/details/111245904
int parseLine(char *line) {
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char *p = line;
    while (*p < '0' || *p > '9') p++;
    line[i - 3] = '\0';
    i = atoi(p);
    return i;
}

int getVmSize(pid_t target_pid) {
    FILE *file;
    char path[128];
    char line[128];
    int res;
    sprintf(path, "/proc/%d/status", target_pid);
    file = fopen(path, "r");

    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            res = parseLine(line);
            break;
        }
    }
    fclose(file);
    return res;
}

void print_virt_mem(void) {
    int vmem;
    pid_t pid;
    pid = getpid();
    vmem = getVmSize(pid);
    printf("Virtual memory usage: %d KB\n", vmem);
}

int main(void) {
    pid_t pid;
    char* addr[MALLOC_TIME];
    int i = 0;

    pid = getpid();
    printf("test_virt, pid = %d\n", pid);
    print_virt_mem();
    for(i = 0; i < MALLOC_TIME; ++i) {
        addr[i] = (char*) malloc(MEM_SIZE);
        if(!addr[i]) {
            printf("执行 malloc 失败, 错误：%s\n", strerror(errno));
		        return -1;
        }
        printf("%d: 主线程调用malloc后，申请128MB大小得内存，此内存起始地址：0X%p\n", i, addr[i]);
        print_virt_mem();
    }

    for(i = 0; i < MALLOC_TIME; ++i) {
        free(addr[i]);
        printf("%d: 主线程调用free后，申请释放128MB大小得内存，此内存起始地址：0X%p\n", i, addr[i]);
        print_virt_mem();
    }

    getchar();
    return 0
}

// gcc -o test_virt2 test_virt2.c  && ./test_virt2
// top -p <pid>
// export LD_PRELOAD=/usr/local/lib/libjemalloc.so
// unset LD_PRELOAD
// lsof -n | grep <pid> | grep jemalloc