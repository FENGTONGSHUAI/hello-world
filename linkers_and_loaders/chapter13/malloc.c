// malloc.c
#include "minicrt.h"

typedef struct _heap_header
{
    enum {
        HEAP_BLOCK_FREE = 0xABABABAB,
        HEAP_BLOCK_USED = 0xCDCDCDCD,
    } type;

    unsigned size;
    struct _heap_header* next;
    struct _heap_header* prev;
} heap_header;

#define ADDR_ADD(a, o) (((char *)(a)) + o)
#define HEADER_SIZE (sizeof(heap_header))

static heap_header* list_head = NULL;

void free(void* ptr) 
{
    heap_header* header = (heap_header*) ADDR_ADD(ptr, -HEADER_SIZE);
    if(header->type != HEAP_BLOCK_USED)
        return;
    
    header->type = HEAP_BLOCK_FREE;
    if(header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE)
    {
        // merge
        header->prev->next = header->next;
        if(header->next != NULL) {
            header->next->prev = header->prev;
        }

        header->prev->size += header->size;
        header = header->prev;
    }

    if(header->next != NULL && header->next->type == HEAP_BLOCK_FREE)
    {
        // merge
        header->size += header->next->size;
        header->next = header->next->next;
    }
}


void *malloc(unsigned size)
{
    heap_header *header;
    if(size == 0) return NULL;

    header = list_head;
    while(header != NULL) 
    {
        if(header->type == HEAP_BLOCK_USED)
        {
            header = header->next;
            continue;
        }

        if(header->size > size + HEADER_SIZE && 
                header->size <= size + HEADER_SIZE * 2) 
        {
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }

        if(header->size > size + HEADER_SIZE * 2) 
        {
            // split
            heap_header* next = (heap_header *)ADDR_ADD(header, size + HEADER_SIZE);
            next->prev = header;
            next->next = header->next;
            next->type = HEAP_BLOCK_FREE;
            next->size = header->size - (size + HEADER_SIZE);
            header->next = next;
            header->size = size + HEADER_SIZE;
            header->type = HEAP_BLOCK_USED;
            return ADDR_ADD(header, HEADER_SIZE);
        }
        header = header->next;
    }

    return NULL;
}

#ifndef WIN32
// Linux brk system call
#if defined(__i386__)
static int brk(void* end_data_segment)
{

    int ret = 0;
    // brk system call number: 45
    // in /usr/include/asm-i386/unistd.h:
    //#define __NR_brk 45
    asm("movl $45, %%eax    \n\t"
        "movl %1, %%ebx     \n\t"
        "int $0x80          \n\t"
        "movl %%eax, %0     \n\t"
        : "=r"(ret): "m"(end_data_segment));
    return ret;  
}
#elif defined(__x86_64__)
static int brk(void* end_data_segment) {  
    int ret;  
    // x86_64 架构下，brk 的系统调用号通常在 sys/syscall.h 中定义为 SYS_brk  
    asm volatile (  
        "movq %1, %%rdi  \n\t" // 将 end_data_segment 的地址放入 %rdi 寄存器  
        "movq $%2, %%rax \n\t" // 将系统调用号 SYS_brk 放入 %rax 寄存器  
        "syscall         \n\t" // 执行系统调用  
        "movq %%rax, %0  \n\t" // 将系统调用的返回值（即错误码）放入 ret 变量  
        : "=r"(ret)            // 输出部分：将 %rax 的值赋给 ret  
        : "r"(end_data_segment), // 输入部分：将 end_data_segment 的值放入 %rdi  
          "i"(12)          // 立即数输入：将系统调用号作为立即数放入  
    );  
    return ret;  
} 
#endif
#endif

#ifdef WIN32
#include <Windows.h>
#endif

int mini_crt_heap_init()
{
    void* base = NULL;
    heap_header *header = NULL;
    // 32 MB heap size
    unsigned heap_size = 1024 * 1024 * 32;

#ifdef WIN32
    base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(base == NULL) {
        return 0;
    }
#else
    base = (void *) brk(0);
    void* end = ADDR_ADD(base, heap_size);
    end = (void *)brk(end);
    if(!end) 
    {
        return 0;
    }
#endif

    header = (heap_header*) base;
    header->size = heap_size;
    header->type = HEAP_BLOCK_FREE;
    header->next = NULL;
    header->prev = NULL;

    list_head = header;
    return 1;
}



