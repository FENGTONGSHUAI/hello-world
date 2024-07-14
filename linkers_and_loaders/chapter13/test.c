#include "minicrt.h"

int main(int argc, char* argv[])
{
    int i;
    FILE* fp;
    printf("%d\n", 1);
    printf("argc = %d, sizeof(char *) = %d, * = %d\n", argc, sizeof(char *), argc * sizeof(char *));
    char** v = malloc(argc * sizeof(char *));
    for(i = 0; i < argc; ++i) {
        v[i] = (char *)malloc(strlen(argv[i]) + 1);
        strcpy(v[i], argv[i]);
        printf("11111 v[0] = %s\n", v[0]);
    }

    printf("2222222\n");
    fp = fopen("test.txt", "w");
    for(i = 0; i < argc; ++i) {
        int len = strlen(v[i]);
        fwrite(&len, 1, sizeof(int), fp);
        fwrite(v[i], 1, len, fp);
    }

    fclose(fp);

    printf("33333333\n");
    fp = fopen("test.txt", "r");
    for(i = 0; i < argc; ++i) {
        int len;
        char* buf;
        fread(&len, 1, sizeof(int), fp);
        printf("4444444\n");
        buf = malloc(len + 1);
        printf("5555555\n");
        fread(buf, 1, len, fp);
        printf("6666666\n");
        buf[len] = '\0';
        printf("7777777\n");
        //下面这行printf会有bug, 后面修一下
        printf("%d %s\n", len, buf);
        printf("8888888\n");
        free(buf);
        free(v[i]);
        
    }
    fclose(fp);
}

/*
gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector test.c
ld -static -e mini_crt_entry entry.o test.o minicrt.a -o test
*/