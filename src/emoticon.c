#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Dot_graphic_library.h"

int main(void)
{
    Line lines[4] = {{.startX = 5, .startY = 1, .stopX = 1, .stopY = 5},
                     {.startX = 4, .startY = 1, .stopX = 2, .stopY = 5},
                     //{.startX = 3, .startY = 1, .stopX = 3, .stopY = 5}, //y parerel
                     {.startX = 2, .startY = 1, .stopX = 4, .stopY = 5},
                     {.startX = 1, .startY = 1, .stopX = 5, .stopY = 5}};

    int cur = 0;
    while(1){
        DOT_Draw_Line(&lines[(cur++)%4]);
        //sleep(1);
    }

    Point points_[7] = {{.X = 1, .Y = 4},
                        {.X = 2, .Y = 1},
                        {.X = 3, .Y = 3},
                        {.X = 2, .Y = 4},
                        {.X = 3, .Y = 2},
                        {.X = 4, .Y = 1},
                        {.X = 4, .Y = 4}};

    Point points[5] = {{.X = 1, .Y = 5},
                       {.X = 2, .Y = 4},
                       {.X = 3, .Y = 2},
                       {.X = 3, .Y = 3},
                       {.X = 4, .Y = 1}};

    // RowBits Frames[DOT_SIZE];

    // printf("        01234567\n");
    // for (int i = 0; i < DOT_SIZE; i++)
    // {
    //     printf("row %d : ", i);
    //     showBinary(Frames[i]);
    // }
    return 0;
}
