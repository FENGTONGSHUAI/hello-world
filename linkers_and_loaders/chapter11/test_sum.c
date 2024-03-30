#include <stdio.h>

int sum(unsigned num, ...) {
    int *p = &num + 1;
    int ret = 0;
    while(num) {
        ret += *p;
        p++;
        num--;
    }
    return ret;
}

int main(void ) {
    int ret;
    ret = sum(3, 16, 38, 53);
    printf("%d\n", ret);

    return 0;
}

/*
可以查看x64的汇编, 并不是通过栈来传递参数的，而是通过寄存器。因此这个程序x64编译是跑不出正确答案的
gcc -m32 -o  test_sum test_sum.c
wsl上不能正常运行, 虚拟机上可以
*/
