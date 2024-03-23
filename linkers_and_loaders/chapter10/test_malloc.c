#include <stdlib.h>

int main()
{
    char * p = (char *)malloc(1000);
    free(p);
}

/*
gcc -S test_malloc.c -o test_malloc.s
gcc -o test_malloc test_malloc.c
*/