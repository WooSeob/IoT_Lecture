
#include "project.h"

#define BEFORE_DOWNLOAD -1000
#define DOWNLOADING -4000
#define DOWNLOAD_FAILURE -1

#define BEFORE_TIMER_SET -10


int EMOTICON_TIMER_MOVE = 0;

int prevTime = -1;
int currentMode = MODE_CLOCK;
int FrameCount = 0;
int WeatherValue = BEFORE_DOWNLOAD;
int GET_WEATHER_ASYNC_FD[2];
int prevTactSwitchInput = 0;

#define TIMER_OK_COMMAND 12
int REMAIN_TIME = BEFORE_TIMER_SET;
int TIMER_INPUT[4] = {-1, -1, -1, -1};

void init_mode(int mode){
    restore_output(mode);
    switch (mode)
    {
    case MODE_CLOCK:
        if(prevTime > 0){
            PrintToFND(prevTime);
        }
        /* code */
        PrintToCLCD("Clock Mode");
        break;
    case MODE_WEATHER:
        /* code */
        PrintToCLCD("Weather Mode");
        break;
    case MODE_TIMER:
        PrintToCLCD("Clock Mode");
        REMAIN_TIME = BEFORE_TIMER_SET;
        PrintToFND(0);
        break;
    default:
        break;
    }
}

int main(void)
{
    restore_output(currentMode);
    while (True)
    {
        // 1Frame(1 * 0.1s) = 0.1초 마다 스위치 감지
        int TS_input =  ScanFromTS(currentMode);

        int SelectedMode = isModeChanged(currentMode, TS_input);
        if (SelectedMode)
        {
            currentMode = SelectedMode;
            // 모드 변경
            init_mode(currentMode);
        }

        //10Frame(10 * 0.1s) = 1초 마다 기능 실행
        switch (currentMode)
        {
        case MODE_CLOCK:
            ClockFunction(TS_input);
            break;

        case MODE_WEATHER:
            WeatherFunction(TS_input);
            break;

        case MODE_TIMER:
            TimerFunction(TS_input);
            break;

        default:
            break;
        }

        usleep(100 * MS); //100ms = 0.1s

    }
    return 0;
}

void ClockFunction(int TS_input)
{
    restore_output(MODE_CLOCK);
    if (FrameCount > 9)
    {
        FrameCount = 0;

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        if (prevTime % 100 != tm.tm_min)
        {
            int Now = tm.tm_hour * 100 + tm.tm_min;
            PrintToFND(Now);
            printf("now: %d\n", Now);
            prevTime = Now;
        }
    }
    else
    {
        FrameCount++;
    }
}

void WeatherFunction(int TS_input)
{
    restore_output(MODE_WEATHER);
    // 프로그램 시연을 위해 임의로 날씨 선택가능
    if(TS_input >= 1 && TS_input <= 5){
        // 1: 맑음, 2: 흐림, 3: 비, 4: 비&눈, 5: 눈
        WeatherValue = TS_input;
    }

    // 0.7초마다 이모티콘 그림 갱신
    if (FrameCount > 6)
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
            // 날씨 애니메이션 출력 
            playEmoticon(WeatherValue);
        }
        FrameCount = 0;
    }
    else
    {
        FrameCount++;
    }
}

void TimerFunction(int TS_input)
{
    restore_output(MODE_TIMER);
    int TimerInput = GetTimerSetValue(TS_input);
    if (REMAIN_TIME == BEFORE_TIMER_SET)
    {
        //타이머 안도는중 -> 시간 세팅받기
        if (TimerInput == TIMER_OK_COMMAND)
        {
            int TimeValue = DigitsToDec(TIMER_INPUT, 4);
            if(TimeValue > 0){
                printf("Timer Start \n");
                // 타이머 시작
                REMAIN_TIME = TimeValue; // 버퍼 이동
                InitDigitsArray(TIMER_INPUT, 4);           //초기화
            }
            return;
        }
        else
        {
            if (TimerInput == MODE_CLOCK || TimerInput == MODE_WEATHER || TimerInput == 0)
            {
                return; //쓰레기값 거르기
            }
            printf("Timer Input \n");

            // 입력 계속받기 inputValue : 1 2 3 4 5 6 7 8 9
            if (TIMER_INPUT[3] != -1)
            {
                int i; // i : 3 2 1
                for (i = 0; i < 3; i++)
                {
                    TIMER_INPUT[i] = TIMER_INPUT[i + 1];
                }
                TIMER_INPUT[3] = TimerInput;
                //넘치면 밀어내기
                PrintToFND(DigitsToDec(TIMER_INPUT, 4));
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
                TIMER_INPUT[i] = TimerInput;
                PrintToFND(DigitsToDec(TIMER_INPUT, 4));
                //들어갈 자리에 넣기
            }
            PrintToConsole(DigitsToDec(TIMER_INPUT, 4));
        }
    }
    else
    {
        //타이머 도는중 -> 끝났는지 확인하기, 캔슬입력 받기
        if (TimerInput == TIMER_OK_COMMAND)
        {
            printf("Timer Cancle\n");
            //타이머 캔슬 -> 초기화
            EMOTICON_TIMER_MOVE = 0;
            REMAIN_TIME = BEFORE_TIMER_SET;
            InitDigitsArray(TIMER_INPUT, 4);
            PrintToConsole(0);
            PrintToFND(0);
            return;
        }

        if (REMAIN_TIME > 0)
        {
            //타이머 도는중
            if (FrameCount > 9)
            {
                // 1초마다 실행
                printf("Timer Count Down \n");
                REMAIN_TIME--;
                PrintToConsole(REMAIN_TIME);
                PrintToFND(REMAIN_TIME);
                FrameCount = 0;
            }
            else
            {
                FrameCount++;
            }
        }
        else
        {
            printf("Time OVER \n");
            

            playTimeover(EMOTICON_TIMER_MOVE);
            if(EMOTICON_TIMER_MOVE==0){
                EMOTICON_TIMER_MOVE++;
            }
            if(EMOTICON_TIMER_MOVE > 64){
                    EMOTICON_TIMER_MOVE = 0;
            }else{
                    EMOTICON_TIMER_MOVE++;
            } 

            if(FrameCount > 4){
                
                FrameCount = 0;
            }else{
                FrameCount++;
            }
            //타이머 OVER
        }
    }
}

int isModeChanged(unsigned char currentMode, int TS_input)
{
    if (TS_input != currentMode && (TS_input == MODE_CLOCK || TS_input == MODE_WEATHER || TS_input == MODE_TIMER))
    {
        printf("MODE CHANGE current : %d, Change To : %d \n", currentMode, TS_input);
        return TS_input;
    }
    else
    {
        return MODE_UNCHANGED;
    }
}

int DigitsToDec(int Value[], int len)
{
    int NumSize, i;
    int value = 0;
    for (NumSize = 0; NumSize < len; NumSize++)
    {
        if (Value[NumSize] == -1)
        {
            break;
        }
    }

    NumSize--;
    for (i = 0; i <= NumSize; i++)
    {
        value += Value[i] * pow(10, NumSize - i);
        printf("DigitsToDec : current value : %d\n", value);
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
int GetTimerSetValue(int TS_input)
{
    int InputValue;
    InputValue = TS_input;

    if (prevTactSwitchInput == InputValue)
    {
        InputValue = 0;
    }
    else
    {
        prevTactSwitchInput = InputValue;
    }

    return InputValue;
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
        int getValue = GetWeather(); // -1 or 1 이상의 값
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