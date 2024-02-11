#include <stdio.h>

int main()
{
    printf("Hello World!\n");
    return 0;
}


// gcc hello.c
// gcc -E hello.c -o hello.i
// gcc -S hello.i -o hello.s
// gcc -S hello.c -o hello.s
// /usr/lib/gcc/x86_64-linux-gnu/7.5.0/cc1 hello.c
// as hello.s -o hello.o
// gcc -c hello.s -o hello.o
// gcc -c hello.c -o hello.o
// ld -static crt1.o crti.o crtbeginT.o hello.o -start-group -lgcc -lgcc-eh -lc -end-group crtend.o crtn.o
