void remdiv(unsigned  x, unsigned long y, unsigned long *qp, unsigned long *rp) {
    unsigned long q = x/y;
    unsigned long r = x%y;
    *qp = q;
    *rp = r;
}

// gcc -Og -S remdiv.c