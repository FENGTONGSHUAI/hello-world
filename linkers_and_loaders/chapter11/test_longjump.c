#include <setjmp.h>
#include <stdio.h>
jmp_buf b;

void f()
{
    longjmp(b, 1);
}

int main()
{
    if (setjmp(b)) {
        printf("World!\n");
    } else {
        printf("Hello ");
        f();
    }

    return 0;
}

/*
gcc -o test_longjump  test_longjump.c
*/