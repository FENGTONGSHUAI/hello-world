gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector entry.c malloc.c stdio.c string.c printf.c
ar -rs minicrt.a malloc.o printf.o stdio.o string.o
