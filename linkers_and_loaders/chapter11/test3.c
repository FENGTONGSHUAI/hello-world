#include <stdio.h>
#include <stdlib.h>

void foo(void)
{
    printf("bye!\n");
}

int main()
{
    atexit(&foo);
    printf("endof main\n");
}

/*
gcc -o test3 test3.c
*/