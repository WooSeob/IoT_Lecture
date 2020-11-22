#define MODE_UNCHANGED 0

#define MODE_CLOCK 10
#define MODE_WHEATHER 11
#define MODE_TIMER 12

#define WHEATHER_SUNNY 1
#define WHEATHER_CLOUDY 2
#define WHEATHER_RAINY 3
#define WHEATHER_SNOWY 4
#define WHEATHER_RAIN_AND_SNOW 5

#define True 1
#define False 0


void TimerFunction();
int isModeChanged(unsigned char currentMode);
void WheatherFunction();
void ClockFunction();
