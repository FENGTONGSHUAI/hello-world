#include "minicrt.h"

#ifdef WIN32
#include <Windows.h>
#endif

extern int main(int argc, char* argv[]);
void exit(int);

static void crt_fatal_error(const char* msg)
{
    printf("fatal error: %s", msg);
    exit(1);
}


void mini_crt_entry(void)
{
    int ret;

#ifdef WIN32
    int flag = 0;
    int argc = 0;
    char * argv[16];
    char* cl = GetCommandLineA();

    // 解析命令行
    argv[0] = cl;
    argc++;
    while(*cl) {
        if(*cl == '\"') {
            if(flag == 0) {
                flag = 1;
            } else {
                flag = 0;
            }
        } else if(*cl == ' ' && flag == 0) {
            if(*(cl + 1)) {
                argv[argc] = cl + 1;
                argc++;
            }
            *cl = '\0';
        }
        cl++;
    }
#else
    int argc;
    char** argv;

#if defined(__i386__)
    char* ebp_reg = 0;
    // ebp_reg = %ebp
    asm("movl %%ebp, %0 \n":"=r"(ebp_reg));

    argc = *(int *)(ebp_reg + 4);
    argv = (char **)(ebp_reg + 8);
#elif defined(__x86_64__)
    char *rbp_reg;
    __asm__ __volatile__("mov %%rbp, %0" : "=r"(rbp_reg));
    argc = *(long *)(rbp_reg + 8);
    argv = (char **)(rbp_reg + 16);
#endif
#endif

    if(!mini_crt_heap_init()) {
        crt_fatal_error("heap initialize failed");
    }

    if(!mini_crt_io_init()) {
        crt_fatal_error("IO initialize failed");
    }
    
    ret = main(argc, argv);

    exit(ret);
}

void exit(int exitCode) {
    // mini_crt_call_exit_routine();
#ifdef WIN32
    ExitProcess(exitCode);
#elif defined(__i386__)
    asm("movl %0, %%ebx \n\t"
        "movl $1, %%eax \n\t"
        "int $0x80      \n\t"
        "hlt            \n\t"::"m"(exitCode)
        );
#elif defined(__x86_64__)
    asm volatile (  
        "movq %0, %%rdi  \n\t" // 将退出码放入 %rdi  
        "movq $%1, %%rax \n\t" // 将系统调用号 SYS_exit 放入 %rax  
        "syscall         \n\t" // 执行系统调用  
        :  
        : "r"(exitCode), "i"(60) // 输入：exitCode 到 %rdi, SYS_exit 到 %rax  
        : "rdi", "rax", "memory", "cc" // 告诉编译器这些寄存器被修改了，以及可能有内存访问和条件码改变  
    );  
#endif
}
