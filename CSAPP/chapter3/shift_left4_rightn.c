long shift_left4_rightn(long x, long n) {
    x <<= 4;
    x >>= n;
    return x;
}

//gcc -Og -S shift_left4_rightn.c