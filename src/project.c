#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "project.h"
#include "io.h"
#include "weather.h"
// #include "Dot_graphic_library.h"
// #include "Lib.h"

#define BEFORE_DOWNLOAD -1000
#define DOWNLOADING -4000

int prevMin = -1;
int currentMode = MODE_CLOCK;
int FrameCount = 0;
int WeatherValue = BEFORE_DOWNLOAD;
int GET_WEATHER_ASYNC_FD[2];

int main(void)
{
    init_TACTSW();

    init_io(currentMode);
    while (True)
    {
        //1Frame(1 * 0.1s) = 0.1초 마다 스위치 감지
        int SelectedMode = isModeChanged(currentMode);
        if (SelectedMode)
        {
            currentMode = SelectedMode;
            close_io();
            init_io(currentMode);
            //모드 변경
        }

        //10Frame(10 * 0.1s) = 1초 마다 기능 실행
        switch (currentMode)
        {
        case MODE_CLOCK:
            ClockFunction();
            break;

        case MODE_WHEATHER:
            WeatherFunction();
            break;

        case MODE_TIMER:
            TimerFunction();
            break;

        default:
            break;
        }

        usleep(100 * MS); //100ms = 0.1s
    }
    return 0;
}

void ClockFunction()
{
    if (FrameCount > 9)
    {
        FrameCount = 0;

        char Message[MAXCHR] = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEF";
        PrintToCLCD(Message);

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        if (prevMin != tm.tm_min)
        {
            int Now = tm.tm_hour * 100 + tm.tm_min;
            PrintToFND(Now);
            printf("now: %d\n", Now);
            prevMin = tm.tm_min;
        }
    }
    else
    {
        FrameCount++;
    }
}

void WeatherFunction()
{
    if (FrameCount > 9)
    {
        printf("\nWeather Function, Weather Value : %d \n", WeatherValue);
        if (WeatherValue == BEFORE_DOWNLOAD)
        { 
            // 한번도 로딩된적없으면
            GetWeatherAsync();
        }
        else if (WeatherValue == DOWNLOADING)
        {
            printf("Weather Data is Downloading...\n");
            read(GET_WEATHER_ASYNC_FD[0], &WeatherValue, sizeof(int));
            printf("read value is %d\n", WeatherValue);
        }
        else
        {
            printf("weather is %d\n", WeatherValue);
            // 날씨 애니메이션 출력
        }

        FrameCount = 0;
    }
    else
    {
        FrameCount++;
    }
}

void TimerFunction()
{
    printf("Timer Function\n");

    int n = 0, count = 0;
    int iCount = 0;
    unsigned char buf[MAXFND];

    // 4자리수에 각각 입력 될 숫자 입력
    int digit;

    // 4자리수에 각각 입력 될 숫자 입력
    PrintToFND(digit);
    // 적절한 delay 사용
    iCount = (iCount + 1) % 10000;
}

int isModeChanged(unsigned char currentMode)
{
    unsigned char ModeValue;

    ModeValue = ScanFromTS();

    if (ModeValue != currentMode && (ModeValue == MODE_CLOCK || ModeValue == MODE_WHEATHER || ModeValue == MODE_TIMER))
    {
        printf("MODE CHANGE current : %d, Change To : %d \n", currentMode, ModeValue);
        return ModeValue;
    }
    else
    {
        return MODE_UNCHANGED;
    }
}

void GetWeatherAsync()
{
    printf("Weather Data is going to Download\n");

    if (pipe(GET_WEATHER_ASYNC_FD) < 0)
    {
        perror("pipe error : ");
        exit(0);
    }

    __pid_t pid;
    pid = fork();

    if (pid == -1)
    {
        printf("fork error \n");
        return;
    }

    if (pid == 0)
    {
        //child
        printf("Get Weather data Async\n");
        int getValue = GetWeather();
        write(GET_WEATHER_ASYNC_FD[1], &getValue, sizeof(int));
        printf("Data Arrived : %d, Process Exit\n", getValue);
        exit(0);
    }
    else
    {
        //parent
        WeatherValue = DOWNLOADING;
    }
}