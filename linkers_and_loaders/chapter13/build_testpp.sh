g++  -c -ggdb -nostdinc++ -fno-rtti -fno-exceptions -fno-builtin -nostdlib -fno-stack-protector testpp.cpp
ld -static -e mini_crt_entry entry.o crtbegin.o testpp.o minicrt.a crtend.o -o testpp