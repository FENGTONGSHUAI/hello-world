#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#define SETUP_STACK                     \
i = 2;                                  \
while(++i < argc - 1) {                 \
    switch (argv[i][0])                 \
    {                                   \
        case 'i':                       \
            asm volatile("push %0" ::   \
            "r"(atoi(&argv[i][1])));    \
            esp += 8;                   \
            break;                      \
        case 'd':                       \
            atof(&argv[i][1]);          \
            asm volatile("subl $8, %esp\n"  \
            "fstpl (%esp)" );           \
            esp += 8;                   \
            break;                      \
        case 's':                       \
            asm volatile("push %0" ::   \
            "r"(&argv[i][1])    );      \
            esp += 8;                   \
            break;                      \
        default:                        \
            printf("error argument type");  \
            goto exit_runso;                 \
    }                                   \
}                                       

#define RESTORE_STACK                   \
    asm volatile("add %0, %%esp"::"r"(esp))

int main(int argc, char* argv[])
{
    void* handle;
    char* error;
    int i;
    int esp = 0;
    void * func;

    handle = dlopen(argv[1],RTLD_NOW);
    if(handle == NULL) {
        printf("Can't find library: %s\n", argv[1]);
        return -1;
    }

    func = dlsym(handle, argv[2]);
    if((error = dlerror()) != NULL) {
        printf("Find symbol %s error: %s\n", argv[2], error);
        goto exit_runso;
    }

    switch (argv[argc - 1][0])
    {
        case 'i':;
        {
            int (*func_int)() = func;
            SETUP_STACK;
            int ret = func_int();
            RESTORE_STACK;
            printf("ret = %d\n", ret);
            break;
        }
        case 'd':;
        {
            double (*func_double)() = func;
            SETUP_STACK;
            double ret = func_double();
            RESTORE_STACK;
            printf("ret = %f\n", ret);
            break;
        }
        case 's':;
        {
            char * (*func_str)() = func;
            SETUP_STACK;
            char* ret = func_str();
            RESTORE_STACK;
            printf("ret = %s\n", ret);
            break;
        }
        case 'v':;
        {
            void (*func_void)() = func;
            SETUP_STACK;
            func_void();
            RESTORE_STACK;
            printf("ret = void\n");
        }
    }

    exit_runso:
    dlclose(handle);
}

/*
gcc -m32 -o RunSo RunSo.c -ldl
./RunSo ./a1.so a v
没有成功改写成64位程序, 直接把a1.so编译成32位动态链接库然后执行a
WSL中运行不成功, 虚拟机上可以
*/
