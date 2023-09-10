#include <stdio.h>

/*
通过真值表可知
a^b^a = b
*/
void inplace_swap(int *x, int *y) {
    if (x == y) {
        printf("inplace_swap, x is same as y, x = %p, y = %p\n", x, y);
        return;
    }
    *y = *x ^ *y;
    *x = *x ^ *y;
    *y = *x ^ *y;
}

void reverse_array(int a[], int cnt) {
    int first, last;
    for(first = 0, last = cnt - 1; first <= last; first++, last--) {
        inplace_swap(&a[first], &a[last]);
    }
}

void printf_int_array(int a[], int cnt) {
    int i;
    for(int i = 0; i < cnt; ++i) {
        printf("%d: %d\n", i, a[i]);
    }
}

int main(void) {
    int a[] = {0, 1, 2, 3, 4};
    printf("before reverse:\n");
    printf_int_array(a, 5);
    reverse_array(a, 5);
    printf("after reverse:\n");
    printf_int_array(a, 5);
}

// gcc -o  inplace_swap inplace_swap.c