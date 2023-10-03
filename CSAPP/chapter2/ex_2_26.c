#include <stdio.h>
#include <string.h>

int strlonger(char *s, char *t) {
    size_t len_s = strlen(s);
    size_t len_t = strlen(t);
    size_t len_s_t = len_s - len_t;

    printf("len_s = %lu, len_t = %lu, len_s_t = %lu\n", len_s, len_t, len_s_t);

    return len_s > len_t;
}

int main(void) {
    char * s = "a";
    char * t = "bc";

    int ret = strlonger(s, t);
    printf("ret = %d\n", ret);

    return 0;
}

// gcc -o  ex_2_26 ex_2_26.c