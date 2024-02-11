/*
* Linux
*   gcc -m32 -c SimpleSection.c
*   gcc -m32 -c -g SimpleSection.c
*/


int printf(const char* format, ...);

int global_init_var = 84;
int global_uninit_var;

void func1(int i) 
{
    printf("%d\n", i);
}

int main(void) 
{
    static int static_var = 85;
    static int static_var2;
    
    int a = 1;
    int b;
    
    func1(static_var + static_var2 + a + b);
    return a;
}

/*
objdump -h SimpleSection.o
size SimpleSection.o
objdump -s -d SimpleSection.o
objdump -x -s -d SimpleSection.o
readelf -h SimpleSection.o
readelf -S SimpleSection.o
hexdump -C -n 82 -s 0x3a0 SimpleSection.o
nm SimpleSection.o
readelf -s SimpleSection.o
readelf -a SimpleSection.o
objdump -t SimpleSection.o

*/

