#include <stdio.h>

float sum_element(float a[], int length) {
    int i = 0;
    float result = 0;
    unsigned int length_1 = length - 1;
    printf("length - 1 = %u\n", length_1);
    for(i = 0; i <= length - 1; ++i) {
        result += a[i];
    }

    return result;
}

int main(void) {
    float a[0];
    sum_element(a, 0);

    return 0;
}

// gcc -o  ex_2_25 ex_2_25.c