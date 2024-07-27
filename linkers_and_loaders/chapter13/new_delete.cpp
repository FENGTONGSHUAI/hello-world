/*
file_name: new_delete.cpp
*/
extern "C" void* malloc(unsigned int);
extern "C" void free(void *);

void * operator new(long unsigned int size)
{
    return malloc(size);
}

void operator delete(void* p)
{
    free(p);
}

void operator delete(void* p, unsigned long)
{
    free(p);
}

void * operator new[](long unsigned int size)
{
    return malloc(size);
}

void operator delete[](void* p)
{
    free(p);
}

void operator delete[](void* p, unsigned long)
{
    free(p);
}