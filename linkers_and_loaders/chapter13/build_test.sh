gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector test.c
ld -static -e mini_crt_entry entry.o test.o minicrt.a -o test
