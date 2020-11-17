#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Dot_graphic_library.h"

int main(void)
{
    Line l;
    l.startX = 4;
    l.startY = 1;
    l.stopX = 1;
    l.stopY = 5;

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

    RowBits Frames[DOT_SIZE];
    Generate_Hex_Code(Frames, points, 5);

    printf("        01234567\n");
    for (int i = 0; i < DOT_SIZE; i++)
    {
        printf("row %d : ", i);
        showBinary(Frames[i]);
    }

    return 0;
}
