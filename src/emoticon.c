#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Dot_graphic_library.h"

#define TEST_SIZE 5
int main(void)
{
    // Line lines[TEST_SIZE] = {{.start = {.X = 5, .Y = 1}, .stop = {.X = 1, .Y = 5}},
    //                          {.start = {.X = 4, .Y = 1}, .stop = {.X = 2, .Y = 5}},
    //                          {.start = {.X = 3, .Y = 1}, .stop = {.X = 3, .Y = 5}},
    //                          {.start = {.X = 2, .Y = 1}, .stop = {.X = 4, .Y = 5}},
    //                          {.start = {.X = 1, .Y = 1}, .stop = {.X = 5, .Y = 5}}};

    Circle circle = {.center = {.X = 4, .Y = 4}, .radius = 1};
    int dX = 1;
    int dY = 1;
    while(1){
        DOT_Draw_Circle(&circle);
        if(circle.center.X > 5 || circle.center.X < 2 ){
            dX = -dX;
        }
        if(circle.center.Y > 5 || circle.center.Y < 2 ){
            dY = -dY;
        }
        circle.center.X += dX;
        circle.center.Y += dY;
        //sleep(1);
    }

    // DOT_Draw_Lines(lines, TEST_SIZE);

    // Point points_[7] = {{.X = 1, .Y = 4},
    //                     {.X = 2, .Y = 1},
    //                     {.X = 3, .Y = 3},
    //                     {.X = 2, .Y = 4},
    //                     {.X = 3, .Y = 2},
    //                     {.X = 4, .Y = 1},
    //                     {.X = 4, .Y = 4}};

    // Point points[5] = {{.X = 1, .Y = 5},
    //                    {.X = 2, .Y = 4},
    //                    {.X = 3, .Y = 2},
    //                    {.X = 3, .Y = 3},
    //                    {.X = 4, .Y = 1}};

    // RowBits Frames[DOT_SIZE];

    // printf("        01234567\n");
    // for (int i = 0; i < DOT_SIZE; i++)
    // {
    //     printf("row %d : ", i);
    //     showBinary(Frames[i]);
    // }

    // int cur = 0;
    // while(1){
    //     DOT_Draw_Line(&lines[(cur++)%TEST_SIZE]);
    //     //sleep(1);
    // }
    return 0;
}
