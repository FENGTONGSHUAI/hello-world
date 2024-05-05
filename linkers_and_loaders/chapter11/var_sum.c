/*
此函数由文心一言生成
*/

#include <stdio.h>  
#include <stdarg.h>  
  
// 使用变长参数的函数  
int sum_of_integers(int count, ...) {  
    va_list args;  
    int sum = 0;  
    int i;  
  
    // 初始化变长参数列表  
    va_start(args, count);  
  
    // 遍历并累加所有整数  
    for (i = 0; i < count; i++) {  
        int num = va_arg(args, int);  
        sum += num;  
    }  
  
    // 清理变长参数列表  
    va_end(args);  
  
    return sum;  
}  
  
int main() {  
    // 调用sum_of_integers函数，传入不同数量的整数  
    printf("Sum of 1, 2, 3: %d\n", sum_of_integers(3, 1, 2, 3));  
    printf("Sum of 4, 5, 6, 7: %d\n", sum_of_integers(4, 4, 5, 6, 7));  
    printf("Sum of 8, 9, 10, 11, 12: %d\n", sum_of_integers(5, 8, 9, 10, 11, 12));  
  
    return 0;  
}


/*
gcc -o var_sum var_sum.c
*/