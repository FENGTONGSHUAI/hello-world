#include <stdlib.h>

int main()
{
    while(1) 
    {
        sleep(1000);
    }

    return 0;
}

/*
gcc -static SectionMapping.c -o SectionMapping.elf
readelf -S SectionMapping.elf  -- 查看段信息
readelf -l SectionMapping.elf  -- 查看映射信息
 cat /proc/26844/maps          -- 查看进程虚拟空间分布
*/