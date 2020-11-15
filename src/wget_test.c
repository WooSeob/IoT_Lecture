#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUF_SIZE 4096
#define WGET "/usr/bin/wget"
#define URL "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=4155051000"
int main(void)
{
    int fd[2];
    char buf[BUF_SIZE];

    if (pipe(fd) < 0)
    {
        perror("pipe error : ");
        exit(0);
    }

    __pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        printf("fork error \n");
        return 0;
    }
    
    int result;
    if (pid == 0)
    {
        //child
        dup2(fd[1], 1);
        if(execl(WGET, WGET, "-O-", URL, NULL) == -1){
            printf("exec error\n");
            exit(1);
        }
    }
    else
    {
        //parent
        int status;
        result = wait(&status);

        read(fd[0], buf, BUF_SIZE);
        printf("%s \n", buf);

        exit(0);
    }
    return 0;
}


//\"http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=4155051000\"