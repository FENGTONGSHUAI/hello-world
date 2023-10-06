long vframe(long n, long idx, long *q) {
    long i;
    long *p[n];
    p[0] = &i;
    for( i = 1; i < n ; ++i) {
        p[i] = q;
    }

    return *p[idx];
}

 // gcc -Og -S vframe.c
 // 汇编代码分配空间那一块没看明白, 后面再看 20231005
