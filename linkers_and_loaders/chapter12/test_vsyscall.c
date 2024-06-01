int main() {
    int ret;
    char msg[] = "Hello\n";
    __asm__ volatile (
        "call *%%esi"
        : "=a" (ret)
        : "a" (4),
        "S" ((0xffffe400)),
        "b" ((long) 1),
        "c" ((long) msg),
        "d" ((long) sizeof(msg))
    );
    return 0;
}

/*
gcc -m32 -o test_vsyscall test_vsyscall.c
并没有跑过. 不是很对.
*/