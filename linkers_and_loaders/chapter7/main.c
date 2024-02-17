#include <stdio.h>

void b1();
void b2();

int main()
{
    b1();
    b2();
    while(1) 
    {
        sleep(1000);
    }
    return 0;
}

/*
gcc main.c b1.so b2.so -o main -Xlinker -rpath ./

执行了export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.  -- 把当前路径加入到动态链接库搜索路径
才成功, 后面看一下为什么书里的指令不能成功了

换一下顺序, 即修改指令为:
gcc main.c b2.so b1.so -o main
则都会打印出a2.c
*/