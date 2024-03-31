## 测试目的
验证多线程程序中, 一个线程sleep, 会不会影响其他线程的执行。

## 测试程序
以下程序来自于chatGPT
```c
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
```
编译指令
```sh
gcc test_sleep.c -lpthread -o test_sleep
```

## 结果分析
实验结果:
![Alt text](images/test_sleep/image-1.png)

chatGPT分析:
> 运行上述程序，你会发现两个线程交替打印数字，而不是一个线程完全执行完毕后再执行另一个线程。这证明了调用sleep方法的线程会进入休眠状态，但其他线程仍然可以继续执行

## sleep实现原理
待补充
可参考 https://zhuanlan.zhihu.com/p/619220682
