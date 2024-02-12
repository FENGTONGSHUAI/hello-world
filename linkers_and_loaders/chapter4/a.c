extern int shared;

// void exit()
// {
//     asm( "movq $66,%rdi \n\t"
//          "movq $60,%rax \n\t"
//          "syscall \n\t");
// }

int main()
{
    int a = 100;
    swap(&a, &shared);

    // exit();
}

/*
gcc -c -fno-stack-protector -m32 a.c b.c
ld -m elf_i386 a.o b.o -e main -o ab
objdump -h a.o  -- 查看段信息
objdump -d a.o  -- 反汇编
objdump -r a.o  -- 查看重定位表
ar -t libc.a
objdump -t libc.a  -- 查看符号表
ld -verbose
ld -T link.script

参考资料: https://zhuanlan.zhihu.com/p/150793679
*/