#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
    char buffer[64];
    char* error_message = "open file error\n";
    char* success_message = "open file success\n";
    
    int fd = open("readme.txt", 0, 0);
    if (fd == -1) {
        write(0, error_message, strlen(error_message));
        return -1;
    }

    write(0, success_message, strlen(success_message));

    // read file 
    read(fd, buffer, 64);
    printf("read content: %s", buffer);
    close(fd);
    return 0;
}

/*
gcc -o test_use_syscall test_use_syscall.c
*/