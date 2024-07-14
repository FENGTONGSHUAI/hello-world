// stdio.c
#include "minicrt.h"

int mini_crt_io_init()
{
    return 1;
}

#ifdef WIN32
// win32暂不实现
#else

#if defined(__i386__)
static int open(const char* pathname, int flags, int mode)
{
    int fd = 0;
    asm("movl $5, %%eax     \n\t"
        "movl %1, %%ebx     \n\t"
        "movl %2, %%ecx     \n\t"
        "movl %3, %%edx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t":
        "=m"(fd):"m"(pathname),"m"(flags),"m"(mode));
    return fd;
}

static int read(int fd, void* buffer, unsigned size)
{
    int ret = 0;
    asm("movl $3, %%eax     \n\t"
        "movl %1, %%ebx     \n\t"
        "movl %2, %%ecx     \n\t"
        "movl %3, %%edx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t":
        "=m"(ret):"m"(fd),"m"(buffer),"m"(size));
    return ret;
}

static int write(int fd, const void* buffer, unsigned size)
{
    int ret = 0;
    asm("movl $4, %%eax     \n\t"
        "movl %1, %%ebx     \n\t"
        "movl %2, %%ecx     \n\t"
        "movl %3, %%edx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t":
        "=m"(ret):"m"(fd),"m"(buffer),"m"(size));
    return ret;
}

static int close(int fd)
{
    int ret = 0;
    asm("movl $6, %%eax     \n\t"
        "movl %1, %%ebx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t":
        "=m"(ret):"m"(fd));
    return ret;
}

static int seek(int fd, int offset, int mode)
{
    int ret = 0;
    asm("movl $19, %%eax     \n\t"
        "movl %1, %%ebx     \n\t"
        "movl %2, %%ecx     \n\t"
        "movl %3, %%edx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t":
        "=m"(ret):"m"(fd),"m"(offset),"m"(mode));
    return ret;
}

#elif defined(__x86_64__)
// 以下由问心一言实现, 有修改
static int open(const char* pathname, int flags, int mode)  
{  
    int fd;  
    __asm__  __volatile__ (  
        "movq %1, %%rdi  \n\t" // 文件名 pathname 放入 %rdi  
        "movq %2, %%rsi  \n\t" // 标志 flags 放入 %rsi  
        "movq %3, %%rdx  \n\t" // 模式 mode 放入 %rdx  
        "movq $%4, %%rax \n\t" // 系统调用号 SYS_open 放入 %rax  
        "syscall         \n\t" // 执行系统调用  
        "movq %%rax, %0  \n\t" // 将返回值（文件描述符）放入 fd  
        : "=r"(fd)             // 输出  
        : "r"(pathname), "r"(flags), "r"(mode), "i"(2) // 输入  
    );  
    return fd;  
}  
  
static int read(int fd, void* buffer, int size)  
{  
    int ret;  
    __asm__ __volatile__ (  
        "movq %1, %%rdi  \n\t" // 文件描述符 fd 放入 %rdi  
        "movq %2, %%rsi  \n\t" // 缓冲区 buffer 地址放入 %rsi  
        "movq %3, %%rdx  \n\t" // 大小 size 放入 %rdx  
        "movq $%4, %%rax \n\t" // 系统调用号 SYS_read 放入 %rax  
        "syscall         \n\t" // 执行系统调用  
        "movq %%rax, %0  \n\t" // 将返回值（读取的字节数或错误码）放入 ret  
        : "=r"(ret)            // 输出  
        : "r"(fd), "r"(buffer), "r"(size), "i"(0) // 输入  
    );  
    return ret;  
}  
  
static int write(int fd, const void* buffer, int size)  
{  
    int ret;  
    __asm__ __volatile__ (  
        "movq %1, %%rdi  \n\t" // 文件描述符 fd 放入 %rdi  
        "movq %2, %%rsi  \n\t" // 缓冲区 buffer 地址放入 %rsi  
        "movq %3, %%rdx  \n\t" // 大小 size 放入 %rdx  
        "movq $%4, %%rax \n\t" // 系统调用号 SYS_write 放入 %rax  
        "syscall         \n\t" // 执行系统调用  
        "movq %%rax, %0  \n\t" // 将返回值（写入的字节数或错误码）放入 ret  
        : "=r"(ret)            // 输出  
        : "r"(fd), "r"(buffer), "r"(size), "i"(1) // 输入  
    );  
    return ret;  
}  
  
static int close(int fd)  
{  
    int ret;  
    __asm__ __volatile__ (  
        "movq %1, %%rdi  \n\t" // 文件描述符 fd 放入 %rdi  
        "movq $%2, %%rax \n\t" // 系统调用号 SYS_close 放入 %rax  
        "syscall         \n\t" // 执行系统调用  
        "movq %%rax, %0  \n\t" // 将返回值（错误码）放入 ret  
        : "=r"(ret)            // 输出  
        : "r"(fd), "i"(3) // 输入  
    );  
    return ret;  
}
  
static int seek(int fd, int offset, int whence)  
{  
    int ret = 0;  
    __asm__ __volatile__ (  
        "movq %1, %%rdi  \n\t" // 文件描述符 fd 放入 %rdi  
        "movq %2, %%rsi  \n\t" // 偏移量 offset 放入 %rsi（注意这里使用mov因为offset是int类型）  
        "movq %3, %%edx  \n\t" // whence（虽然是int，但用%edx传递）  
        "movq $%4, %%rax \n\t" // 系统调用号 SYS_lseek 放入 %rax  
        "syscall         \n\t" // 执行系统调用  
        "movq %%rax, %0  \n\t" // 将返回值（新的偏移量或错误码）放入 ret  
        : "=r"(ret)            // 输出  
        : "r"(fd), "r"(offset), "r"(whence), "i"(8) // 输入  
    );  
    return ret;  
}
#endif

FILE *fopen(const char* filename, const char* mode)
{
    int fd = -1;
    int flags = 0;
    int access = 00700;

    #define O_RDONLY            00
    #define O_WRONLY            01
    #define O_REWR              02

    #define O_CREAT             0100
    #define O_TRUNC             01000
    #define O_APPEND            02000

    if(strcmp(mode, "w") == 0) {
        flags |= O_WRONLY | O_CREAT | O_TRUNC;
    }

    if(strcmp(mode, "w+") == 0) {
        flags |= O_REWR | O_CREAT | O_TRUNC;
    }

    if(strcmp(mode, "r") == 0) {
        flags |= O_RDONLY;
    }

    if(strcmp(mode, "r+") == 0) {
        flags |= O_REWR | O_CREAT;
    }

    fd = open(filename, flags, access);
    return (FILE *) fd;
}

int fread(void * buffer, int size, int count, FILE* stream)
{
    return read((int) stream, buffer, size * count);
}

int fwrite(const void * buffer, int size, int count, FILE* stream)
{
    return write((int) stream, buffer, size * count);
}

int fclose(FILE* fp)
{
    return close((int) fp);
}

int feek(FILE* fp, int offset, int set)
{
    return seek((int)fp, offset, set);
}

#endif
