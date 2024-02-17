#include <stdio.h>

void foobar(int i)
{
    printf("Printing from Lib.so %d\n", i);
    sleep(-1);
}

/*
gcc -fPIC -shared -o Lib.so Lib.c
-shared 表示产生共享对象
readelf -l Lib.so  -- 查看Lib.so的装载属性
objdump -R Lib.so  -- 查看动态库符号表
objdump -s Lib.so  -- 查看动态链接器路径
readelf -l a.out | grep interpreter  -- 查看动态链接器路径
readelf -d Lib.so  -- 查看.dynamic段内容
ldd Program1 --查看程序主模块或共享库依赖哪些共享库
readelf -sD Lib.so -- 查看ELF文件的动态符号表和他的哈希表
readelf -r Lib.so  -- 查看动态链接文件的重定位表
gcc -shared -o Lib.so Lib.c
readelf -r Lib.so   -- 查看动态链接的重定位表
*/