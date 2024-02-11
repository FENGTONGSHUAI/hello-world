#include <stdio.h>
#include <pthread.h>

int pthread_create(
    pthread_t*,
    const pthread_attr_t*,
    void* (*)(void*),
    void*) __attribute__ ((weak));

int main()
{
    if(pthread_create) {
        printf("This is multi-thread version!\n");
    } else {
        printf("This is single-thread version!\n");
    }
}

// gcc weekRef.c -o weekRef  
// gcc weekRef.c -lpthread -o weekRef 
// 并没有测试成功, 不知道为什么。