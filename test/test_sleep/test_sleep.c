#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    for (int i = 0; i < 5; i++) {
        printf("Thread %d: %d\n", thread_num, i);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int num1 = 1, num2 = 2;

    pthread_create(&thread1, NULL, thread_func, &num1);
    pthread_create(&thread2, NULL, thread_func, &num2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

/*
gcc test_sleep.c -lpthread -o test_sleep
*/