char * str = "Hello world!\n";

void print()
{
    asm(
        "movl $13,%%edx \n\t"
        "movl %0,%%ecx \n\t"
        "movl $0,%%ebx \n\t"
        "movl $4,%%eax \n\t"
        "int $0x80     \n\t"
        ::"r"(str):"edx","ecx","ebx"
    );
}

void exit()
{
    asm(
        "movl $42, %ebx \n\t"
        "movl $1, %eax  \n\t"
        "int $0x80      \n\t"
        );
}

void nomain()
{
    print();
    exit();
}

/*
gcc -c -m32 -fno-builtin TinyHelloWorld.c
ld -static -e nomain -m elf_i386 -o TinyHelloWorld TinyHelloWorld.o 
wsl下执行不成功
64位系统下的类似程序可以参考
https://github.com/chenpengcong/blog/issues/13

ld -static -T TinyHelloWorld.lds  -m elf_i386 -o TinyHelloWorld TinyHelloWorld.o 
*/