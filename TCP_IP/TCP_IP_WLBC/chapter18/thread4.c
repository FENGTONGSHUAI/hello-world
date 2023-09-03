#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define NUM_THREAD 100
void* thread_inc(void *arg);
void* thread_des(void *arg);
long long num = 0;

int main(int argc, char *argv[]) {
    pthread_t thread_id[NUM_THREAD];
    int i;

    printf("sizeof long long: %ld \n", sizeof(long long));

    for(i = 0; i < NUM_THREAD; ++i) {
        if(i % 2) {
            if(pthread_create(&(thread_id[i]), NULL, thread_inc, NULL) != 0) {
                puts("pthread_create() error");
                return -1;
            }
        } else {
            if(pthread_create(&(thread_id[i]), NULL, thread_des, NULL) != 0) {
                puts("pthread_create() error");
                return -1;
            }
        }
    }

    for(i = 0; i < NUM_THREAD; ++i) {
        if(pthread_join(thread_id[i], NULL) != 0) {
            puts("pthread_join() error");
            return -1;
        }
    }

    printf("result: %lld \n", num);
    return 0;
}

void* thread_inc(void *arg) {
    int i;
    for(i = 0; i < 50000000; ++i) {
        num += 1;
    }
    return NULL;
}

void* thread_des(void *arg) {
    int i;
    for(i = 0; i < 50000000; ++i) {
        num -= 1;
    }
    return NULL;
}

// gcc -o thread4 thread4.c -D_REENTERANT -lpthread