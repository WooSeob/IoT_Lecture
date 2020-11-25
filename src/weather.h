#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#define BUF_SIZE 4096
#define WGET "/usr/bin/wget"
#define URL "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=4155051000"

#define W_CLEAR ">Clear</wfEn>"
#define W_CLOUDY ">Clear</wfEn>"
#define W_RAIN ">Clear</wfEn>"
#define W_SNOW ">Clear</wfEn>"
#define W_WINDY ">Clear</wfEn>"

#define WEATHER_SUNNY 1
#define WEATHER_CLOUDY 2
#define WEATHER_RAINY 3
#define WEATHER_SNOWY 4
#define WEATHER_RAIN_AND_SNOW 5

int GetWeather();

