#ifndef MINICRT_H_
#define MINICRT_H_

#ifdef __cplusplus
extern "C" {
#endif

// malloc 
#ifndef NULL
#define NULL (0)
#endif

void free(void *ptr);
void *malloc(unsigned size);
static int brk(void* end_data_segment);
int mini_crt_heap_init();

// 字符串
char * itoa(int n, char* str, int radix);
int strcmp(const char * src, const char * dst);
char *strcpy(char *dest, const char *src);
unsigned strlen(const char *str);

void mini_crt_entry(void);

// 文件与IO
typedef int  FILE;
#define EOF (-1)
#ifdef WIN32

#else
#define stdin   ((FILE*) 0)
#define stdout  ((FILE*) 1)
#define stderr  ((FILE*) 2)
#endif

int mini_crt_io_init();
FILE *fopen(const char* filename, const char* mode);
int fread(void * buffer, int size, int count, FILE* stream);
int fwrite(const void * buffer, int size, int count, FILE* stream);
int fclose(FILE* fp);
int feek(FILE* fp, int offset, int set);

// printf
int fputc(int c, FILE *stream);
int fputs(const char *str, FILE *stream);
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);

//internal
void do_global_ctors();
void mini_crt_call_exit_routine();

// atexit
typedef void (*atexit_func_t) (void);
int atexit(atexit_func_t func);

#ifdef __cplusplus
}
#endif


#endif

/*
gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector entry.c malloc.c stdio.c string.c printf.c
ar -rs minicrt.a malloc.o printf.o stdio.o string.o
*/
