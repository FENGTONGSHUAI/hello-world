#include "tongshuai.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void doWriteLog() {
    int i = 0;
    for(i = 0; i < 5; ++i) {
        tongshuaiLog(0, "这是本进程第%d条log", i);
        //sleep(1);
    }
}

void doWriteFile(FILE *fp) {
    int i = 0;
    char msg[TONGSHUAI_MAX_LOGMSG_LEN];
    for(i = 0; i < 10; ++i) {
        sprintf(msg, "这是本进程第%d条log", i);
        fprintf(fp,"[%d] %s\n",(int)getpid(),msg);
    }
}

int main(void)
{
    int i = 0;
    pid_t pid;
    int status;
    FILE *fp;

    printf("I'm start parent, pid = %d, ppid = %d\n", getpid(), getppid());
    fp = fopen("test.txt","w");
    if (!fp) return -1;
    for(;i < 5; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            break; // 要创建6个子进程这里就必须break跳出，否则子进程又会继续创建子进程，结果就会创建 2^6 - 1个子进程
        }else if(pid <0){
            perror("fork error!");
            exit(1);
        }
    }
	// 父进程, 子进程都写日志
    // doWriteLog();
    doWriteFile(fp);

    // 等待所有子进程结束, 参考https://stackoverflow.com/questions/1510922/waiting-for-all-child-processes-before-parent-resumes-execution-unix
    while ((pid = wait(&status)) > 0);
    return 0;
}
