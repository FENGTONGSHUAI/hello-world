#include <stdio.h>
#include "vec.h"
#include <sys/time.h>

int main(void) {
    struct timeval begin, end;
    long seconds, microseconds;
    double elapsed;
    data_t result;
    vec_ptr ptr;
    ptr = new_vec(10000000);

    // combine1
    gettimeofday(&begin, 0);
    combine1(ptr, &result);
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = seconds + microseconds*1e-6;
    printf("combine1 time measured: %.6f seconds.\n", elapsed);

    // combine2
    gettimeofday(&begin, 0);
    combine2(ptr, &result);
    gettimeofday(&end, 0);
    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = seconds + microseconds*1e-6;
    printf("combine2 time measured: %.6f seconds.\n", elapsed);

    return 0;
}