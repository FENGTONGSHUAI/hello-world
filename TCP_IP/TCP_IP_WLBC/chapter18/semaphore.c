#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM_THREAD 100
void* read(void *arg);
void* accu(void *arg);
static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[]) {
    pthread_t id_t1, id_t2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    if(pthread_create(&id_t1, NULL, read, NULL) != 0) {
        puts("pthread_create() error");
        return -1;
    }

    if(pthread_create(&id_t2, NULL, accu, NULL) != 0) {
        puts("pthread_create() error");
        return -1;
    }

    if(pthread_join(id_t1, NULL) != 0) {
        puts("pthread_join() error");
        return -1;
    }

    if(pthread_join(id_t2, NULL) != 0) {
        puts("pthread_join() error");
        return -1;
    }

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

void* read(void *arg) {
    int i;
    for(i = 0; i < 5; ++i) {
        fputs("Input num: ", stdout);
        sem_wait(&sem_two);
        scanf("%d", &num);
        sem_post(&sem_one);
    }
    return NULL;
}

void* accu(void *arg) {
    int sum = 0, i;
    for(i = 0; i < 5; ++i) {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    printf("Result: %d \n", sum);
    return NULL;
}

// gcc -o semaphore semaphore.c -D_REENTERANT -lpthread