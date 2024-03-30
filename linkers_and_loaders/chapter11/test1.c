#include <stdio.h>
#include <stdlib.h>

int a = 3;

int main(int argc, char * argv[])
{
    int *p = (int *)malloc(sizeof(int));
    scanf("%d", p);
    printf("%d\n", a + *p);
    free(p);

    return 0;
}

/*
gcc -o test1 test1.c
*/