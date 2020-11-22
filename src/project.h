#define MODE_UNCHANGED 0

#define MODE_CLOCK 10
#define MODE_WHEATHER 11
#define MODE_TIMER 12

#define WEATHER_SUNNY 1
#define WEATHER_CLOUDY 2
#define WEATHER_RAINY 3
#define WEATHER_SNOWY 4
#define WEATHER_RAIN_AND_SNOW 5

#define True 1
#define False 0


void TimerFunction();
int isModeChanged(unsigned char currentMode);
void WeatherFunction();
void ClockFunction();
void GetWeatherAsync();