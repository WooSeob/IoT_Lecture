#include "weather.h"

int GetWeather()
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
        printf("child process start\n");
        //child
        dup2(fd[1], 1);
        if (execl(WGET, WGET, "-O-", URL, NULL) == -1)
        {
            printf("exec error\n");
            exit(1);
        }
    }
    else
    {
        printf("parent process start\n");
        //parent
        int status;
        result = wait(&status);

        read(fd[0], buf, BUF_SIZE);
        // printf("%s \n", buf);
        char *ptr = strtok(buf, " "); //첫번째 strtok 사용.

        char *targetTAG = "<wfEn>";

        while (ptr != NULL) //ptr이 NULL일때까지 (= strtok 함수가 NULL을 반환할때까지)
        {
            int i;
            char *Searched;
            int len = strlen(targetTAG);
            for (i = 0; i < len; i++)
            {
                Searched = strchr(ptr, targetTAG[i]);
                if (Searched != NULL){
                    if(i == len-1){
                        // 찾음
                        // printf("Tag Searched\n");
                        // printf("%s\n", Searched);

                        if(!strcmp(Searched, W_CLEAR)){
                            return WEATHER_SUNNY;
                        }else if(!strcmp(Searched, W_CLOUDY)){
                            return WEATHER_CLOUDY;
                            // ...
                        }
                    }
                }else{
                    break;
                }
            }

            ptr = strtok(NULL, "\n"); //자른 문자 다음부터 구분자 또 찾기
        }

        return -1;
    }
    return 0;
}