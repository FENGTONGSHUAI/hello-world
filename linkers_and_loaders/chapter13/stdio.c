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
static int64_t open(const char* pathname, int flags, int64_t mode)  
{  
    int fd;  
    __asm__ __volatile__("syscall"
                       : "=a"(fd)
                       : "a"(2), "D"(pathname), "S"(flags),
                         "d"(mode)); 
    return fd;  
}  
  
static int64_t read(int fd, void* buffer, uint64_t size)  
{  
    int64_t ret;  
    __asm__ __volatile__("syscall"
                       : "=a"(ret)
                       : "a"(0), "D"(fd), "S"(buffer), "d"(size)); 
    return ret;  
}  
  
static int64_t write(int fd, const void* buffer, uint64_t size)  
{  
    int64_t ret;  
    __asm__ __volatile__("syscall"
                       : "=a"(ret)
                       : "a"(1), "D"(fd), "S"(buffer), "d"(size));
    return ret;  
}  
  
static int64_t close(int fd)  
{  
    int64_t ret;  
    __asm__ __volatile__("syscall" : "=a"(ret) : "a"(3), "D"(fd));

    return ret;  
}
  
static int64_t seek(int fd, uint64_t offset, int mode)  
{  
    int64_t ret = 0;  
    __asm__ __volatile__("syscall"
                       : "=a"(ret)
                       : "a"(8), "D"(fd), "S"(offset), "d"(mode));
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
