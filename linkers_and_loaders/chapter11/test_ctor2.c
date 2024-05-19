#include <stdio.h>

void my_init(void) __attribute__ ((constructor));
void my_init(void)
{
    printf("Hello ");
}

int main()
{
    printf("World!\n");
    return 0;
}

/*
gcc -o  test_ctor2 test_ctor2.c
*/