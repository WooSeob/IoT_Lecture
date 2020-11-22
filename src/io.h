#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#define fnd_dev "/dev/fnd"
#define clcd_dev "/dev/clcd"
#define dot_dev "/dev/dot"
#define ds_dev ""
#define ts_dev "/dev/tactsw"

#define MAXFND 4
#define MAXCHR 32
#define MS 1000
#define MAXDIGHT 10

unsigned char asc_to_fnd(int n);
void init_io(int MODE);
void close_io();
void init_FND();
void init_CLCD();
void init_TACTSW();
void init_DOT();

void PrintToFND(int digit_4);
void PrintToCLCD(char *string);
void PrintToDOT();
void ScanFromDS();
unsigned char ScanFromTS();