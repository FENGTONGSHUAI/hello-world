gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector entry.c malloc.c stdio.c string.c printf.c atexit.c
g++  -c -ggdb -nostdinc++ -fno-rtti -fno-exceptions -fno-builtin -nostdlib -fno-stack-protector crtbegin.cpp \
    crtend.cpp ctors.cpp new_delete.cpp sysdep.cpp iostream.cpp sysdep.cpp
ar -rs minicrt.a malloc.o printf.o stdio.o string.o ctors.o atexit.o iostream.o new_delete.o sysdep.o
