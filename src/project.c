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
#define DOWNLOAD_FAILURE -1

#define BEFORE_TIMER_SET -10

int prevMin = -1;
int currentMode = MODE_CLOCK;
int FrameCount = 0;
int WeatherValue = BEFORE_DOWNLOAD;
int GET_WEATHER_ASYNC_FD[2];

#define TIMER_OK_COMMAND 11
int REMAIN_TIME = BEFORE_TIMER_SET;
int TIMER_INPUT[4] = {-1, -1, -1, -1};

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
            // 다운로드 시작
            GetWeatherAsync();
        }
        else if (WeatherValue == DOWNLOADING)
        {
            // 다운로드중
            printf("Weather Data is Downloading...\n");
            read(GET_WEATHER_ASYNC_FD[0], &WeatherValue, sizeof(int));
            printf("read value is %d\n", WeatherValue);
        }
        else if (WeatherValue == DOWNLOAD_FAILURE)
        {
            // 다운로드 실패 -> 재다운로드
            printf("Weather Data download failure. download retry.\n");
            WeatherValue = BEFORE_DOWNLOAD;
        }
        else
        {
            // 날씨정보 받아오기 성공
            printf("weather is %d\n", WeatherValue);
            // CLCD 날씨 출력******************************************
            // 날씨 애니메이션 출력 *********************************8*

            Drawable dFrame_1 = {.type = TYPE_Points, .Points = &EMOTICONS[WeatherValue]};
            Drawable dFrame_2 = {.type = TYPE_Points, .Points = &EMOTICONS[WeatherValue + 1]};

            RenderQueue Frame1, Frame2;

            Initialize(&Frame1);
            Initialize(&Frame2);

            InsertFront(&Frame1, dFrame_1);
            InsertFront(&Frame2, dFrame_2);

            // DOT_Draw_Canvas() ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            
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
    int inputValue = GetTimerSetValue();
    if (REMAIN_TIME == BEFORE_TIMER_SET)
    {
        //타이머 안도는중 -> 시간 세팅받기
        if (inputValue == TIMER_OK_COMMAND)
        {
            // 타이머 시작
            REMAIN_TIME = DigitsToDec(TIMER_INPUT, 4); // 버퍼 이동
            InitDigitsArray(TIMER_INPUT, 4);           //초기화
            return;
        }
        else
        {
            if (inputValue == 10 || inputValue == 12)
            {
                return; //쓰레기값 거르기
            }

            // 입력 계속받기 inputValue : 1 2 3 4 5 6 7 8 9
            if (TIMER_INPUT[3] != -1)
            {
                int i; // i : 3 2 1
                for (i = 3; i > 0; i--)
                {
                    TIMER_INPUT[i] = TIMER_INPUT[i - 1];
                }
                TIMER_INPUT[0] = inputValue;
                //넘치면 밀어내기
            }
            else
            {
                int i;
                for (i = 0; i < 4; i++)
                {
                    if (TIMER_INPUT[i] == -1)
                    {
                        break;
                    }
                }
                TIMER_INPUT[i] = inputValue;
                //들어갈 자리에 넣기
            }

            //Interaction 눌른대로 현재 값 출력해주기 *******************************************
            PrintToConsole(DigitsToDec(TIMER_INPUT, 4));
        }
    }
    else
    {
        //타이머 도는중 -> 끝났는지 확인하기, 캔슬입력 받기
        if (inputValue == TIMER_OK_COMMAND)
        {
            //타이머 캔슬
            //초기화
            REMAIN_TIME = BEFORE_TIMER_SET;
            InitDigitsArray(TIMER_INPUT, 4);

            //TODO 0000출력 ****************************************************
            // CLCD "시간을 입력하고 OK 입력해주세요."
            // 여기에 코드 입력
            PrintToConsole(0);
            return;
        }

        if (REMAIN_TIME > 0)
        {
            //타이머 도는중
            if (FrameCount > 9)
            {
                FrameCount = 0;
                // 1초마다 실행
                REMAIN_TIME--;
                //************************* 출력 ******************************
                PrintToConsole(REMAIN_TIME);
            }
        }
        else
        {
            //타이머 OVER
            if (FrameCount > 9)
            {
                FrameCount = 0;
                // 1초마다 실행

                //************************* 출력 ******************************
                // CLCD "OK 눌러주세요"
                // 타이머 오버 출력
            }
        }
    }
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

int DigitsToDec(int Value[], int len)
{
    int i;
    int value = 0;
    for (i = 0; i < len; i++)
    {
        value += Value[i] * pow(10, i);
    }
    return value;
}
void InitDigitsArray(int Value[], int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        Value[i] = -1;
    }
}
int GetTimerSetValue()
{
    int TimeValue;

    TimeValue = ScanFromTS();

    return TimeValue;
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

// printf("Timer Function\n");

//     int n = 0, count = 0;
//     int iCount = 0;
//     unsigned char buf[MAXFND];

//     // 4자리수에 각각 입력 될 숫자 입력
//     int digit;

//     // 4자리수에 각각 입력 될 숫자 입력
//     PrintToFND(digit);
//     // 적절한 delay 사용
//     iCount = (iCount + 1) % 10000;