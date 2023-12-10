#include <stdio.h>
#include "jemalloc.h"

int main() {
    // 初始化jemalloc
    if (je_init() != 0) {
        fprintf(stderr, "Failed to initialize jemalloc\n");
        return 1;
    }
    // 分配内存
    size_t size = 1024; // 分配1KB的内存
    void* ptr = je_malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    // 使用分配的内存
    sprintf(ptr, "Hello, jemalloc!");

    // 打印分配的内存内容
    printf("%s\n", (char*)ptr);

    // 释放内存
    je_free(ptr);

    // 结束jemalloc
    je_uninit();

    return 0;
}

// gcc -o test_jemalloc test_jemalloc.c -ljemalloc