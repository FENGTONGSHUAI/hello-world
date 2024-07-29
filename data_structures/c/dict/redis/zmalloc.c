#include <stdio.h>
#include <stdlib.h>
#include "zmalloc.h"

static void zmalloc_default_oom(size_t size) {
    fprintf(stderr, "zmalloc: Out of memory trying to allocate %zu bytes\n",
        size);
    fflush(stderr);
    abort();
}

void *zmalloc(size_t size)
{
    void *ptr = malloc(size);

    if (!ptr) {
        zmalloc_default_oom(size)
    }

    return ptr;
}

void *zcalloc(size_t size)
{
    void *ptr = calloc(1, size);

    if (!ptr) {
        zmalloc_default_oom(size)
    }

    return ptr;
}

void zfree(void *ptr)
{
    if (ptr == NULL) return;

    free(ptr);
}