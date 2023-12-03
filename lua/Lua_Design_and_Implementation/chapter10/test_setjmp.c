#include <stdio.h>
#include <setjmp.h>

static jmp_buf buf;

void second(void) {
    printf("in second!\n");
    longjmp(buf, -1);
}

void first(void) {
    printf("in first\n");
    second();
    printf("after second\n");
}

int main() {
    int ret = setjmp(buf);
    if(!ret) {
        first();
    } else {
        printf("longjmp ret:%d\n", ret);
    }

    return 0;
}

// gcc -o test_jmp test_setjmp.c