#include "io.h"
#include "project.h"

int FND_FD = -1, DOT_FD = -1, CLCD_FD = -1, DS_FD = -1, TACTSW_FD = -1;

void init_io(int MODE)
{
    switch (MODE)
    {
    case MODE_CLOCK:
        init_FND();
        init_CLCD();
        break;
    case MODE_WHEATHER:
        init_FND();
        init_DOT();
        init_CLCD();
        break;
    case MODE_TIMER:
        init_FND();
        init_CLCD();
        break;
    default:
        break;
    }
}

void close_io()
{
    if (FND_FD >= 0)
    {
        close(FND_FD);
    }

    if (CLCD_FD >= 0)
    {
        close(CLCD_FD);
    }
    if (DOT_FD >= 0)
    {
        close(DOT_FD);
    }
}

void init_FND()
{
    FND_FD = open(fnd_dev, O_RDWR);
    if (FND_FD < 0)
    {
        fprintf(stderr, "cannot open FND (%d)", FND_FD);
        exit(0);
    }
}
void init_CLCD()
{
    CLCD_FD = open(clcd_dev, O_RDWR);
    if (CLCD_FD < 0)
    {
        fprintf(stderr, "cannot open LCD (%d)", CLCD_FD);
        exit(0);
    }
}
void init_TACTSW()
{
    TACTSW_FD = open(ts_dev, O_RDONLY);
    if (TACTSW_FD < 0)
    {
        perror("opening TactSW failure\n");
        exit(-1);
    }
}
void init_DOT(){
    DOT_FD = open(dot_dev, O_RDWR);
    if (DOT_FD < 0)
    {
        perror("opening DOT failure\n");
        exit(-1);
    }
}
unsigned char asc_to_fnd(int n)
{
    unsigned char FND_BITS[11] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x98, 0xFF};
    if (n >= 0 && n < 10)
    {
        //printf("asc_to_fnd n : %d\n", n);
        return FND_BITS[n];
    }
    else
    {
        return FND_BITS[10];
    }
}

void PrintToFND(int digit_4)
{
    unsigned char Hex_Code[MAXFND];
    memset(Hex_Code, 0x00, sizeof(Hex_Code));

    int TargetNum = digit_4;
    int i;
    for (i = 1; i < MAXFND + 1; i++)
    {
        int Num = TargetNum / pow(10, MAXFND - i);
        TargetNum -= Num * pow(10, MAXFND - i);
        Hex_Code[i - 1] = asc_to_fnd(Num);
    }
    write(FND_FD, Hex_Code, MAXFND);
}

void PrintToCLCD(char *s)
{
    int n;
    char buf[MAXCHR];
    memset(buf, 0, sizeof(buf));
    printf("buf : %s\n", buf);  

    n = strlen(s);
    if (n > MAXCHR)
         n = MAXCHR; //plus the newline
    // printf("n : %d\n", n);
    
    memcpy(buf, s, n);
    // printf("buf : %s\n", buf);  
    
    // printf("clcd fd : %d\n", CLCD_FD);
    write(CLCD_FD, buf, MAXCHR);
}

void PrintToDOT()
{
}

void PrintToConsole(int n){
    printf("Print : %d\n", n);
}
void ScanFromDS()
{
}

unsigned char ScanFromTS()
{
    unsigned char readData;
    read(TACTSW_FD, &readData, 2);
    return readData;
}
