// a.c 
void __attribute__((fastcall)) foo(int, int);

int main()
{
    foo(1, 3);
    return 0;
}

// gcc -o test a.c b.c
/*
编译报告警:
a.c:2:1: warning: ‘fastcall’ attribute ignored [-Wattributes]
 void __attribute__((fastcall)) foo(int, int);
 ^~~~
b.c:4:1: warning: ‘cdecl’ attribute ignored [-Wattributes]
 {
 ^
目前没有找到解决办法, 这个测试先不做了, 20240315
*/