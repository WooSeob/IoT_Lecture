#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>

#include "Dot_graphic_library.h"

void MappingLineToDotMatrix(RowBits *Frames, char MappedPoints[][DOT_SIZE], Line *line);
void MappingCircleToDotMatrix(RowBits *Frames, char MappedPoints[][DOT_SIZE], Circle *circle);
void RenderingFrame(RowBits *Frames);
void showBinary(RowBits binary);

// To Abstraction Application <-> 8x8 Dot Matrix
void DOT_Draw_Points(Point points[], int len){
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];
    
    for(int i = 0; i<len; i++){
        Point p = points[i];
        MappedPoints[p.Y][p.X] = 1;
    }

    Generate_Hex_Code(Frames, MappedPoints);

    RenderingFrame(Frames);
}

void DOT_Draw_Circle(Circle *circle)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];

    MappingCircleToDotMatrix(Frames, MappedPoints, circle);
    Generate_Hex_Code(Frames, MappedPoints);
    
    RenderingFrame(Frames);
}

void DOT_Draw_Line(Line *line)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];

    MappingLineToDotMatrix(Frames, MappedPoints, line);
    Generate_Hex_Code(Frames, MappedPoints);
    
    RenderingFrame(Frames);
}

void DOT_Draw_Lines(Line line[], int len)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];

    for(int i = 0; i < len; i++){
        MappingLineToDotMatrix(Frames, MappedPoints, &line[i]);
    }
    Generate_Hex_Code(Frames, MappedPoints);
    
    RenderingFrame(Frames);
}

// <--------------------- Private Functions --------------------->
// 1. Mapping
// 2. Generate Hex code
// 3. Rendering with Hex code

void MappingLineToDotMatrix(RowBits *Frames, char MappedPoints[][DOT_SIZE], Line *line)
{
    
    // y 수평 함수 x = 3
    Point start = line->start, stop = line->stop;
    CoordValue Dx = (stop.X - start.X);
    CoordValue Dy = (stop.Y - start.Y);
    float D, C, rD, rC;

    D = (Dx != 0) ? (float)Dy / (float)Dx : 0; // Dx==0 : y수평함수
    C = (start.Y) - D * (start.X);
    //y = Dx + C

    rD = (Dy != 0) ? (float)Dx / (float)Dy : 0; // Dy==0 : x수평함수
    rC = (start.X) - rD * (start.Y);
    //x = rDy + rC

    //# x기준 매핑
    int lStartX = start.X > stop.X ? stop.X : start.X;
    int lStopX = start.X > stop.X ? start.X : stop.X;
    for (int x = lStartX; x <= lStopX; x++)
    {
        int y = D * x + C;
        MappedPoints[y][x] = 1;
    }

    //# y기준 매핑
    int lStartY = start.Y > stop.Y ? stop.Y : start.Y;
    int lStopY = start.Y > stop.Y ? start.Y : stop.Y;
    for (int y = lStartY; y <= lStopY; y++)
    {
        int x = rD * y + rC;
        MappedPoints[y][x] = 1;
    }
}

void MappingCircleToDotMatrix(RowBits *Frames, char MappedPoints[][DOT_SIZE], Circle *circle)
{
    CoordValue R = circle->radius;
    Point Center = circle->center;

    //# x기준 매핑
    int lStartX = Center.X - R;
    int lStopX = Center.X + R;
    for (int x = lStartX; x <= lStopX; x++)
    {
        int y1 = sqrt(pow(R,2) - pow((x - Center.X), 2)) + Center.Y;
        int y2 = -sqrt(pow(R,2) - pow((x - Center.X), 2)) + Center.Y;
        MappedPoints[y1][x] = 1;
        MappedPoints[y2][x] = 1;
    }

    //# y기준 매핑
    int lStartY = Center.Y - R;
    int lStopY = Center.Y + R;
    for (int y = lStartY; y <= lStopY; y++)
    {
        int x1 = sqrt(pow(R,2) - pow((y - Center.Y), 2)) + Center.X;
        int x2 = -sqrt(pow(R,2) - pow((y - Center.Y), 2)) + Center.X;
        MappedPoints[y][x1] = 1;
        MappedPoints[y][x2] = 1;
    }
}

void Generate_Hex_Code(RowBits *Frame, char points[][DOT_SIZE])
{
    for (int i = 0; i < DOT_SIZE; i++)
    {
        Frame[i] = 0x00;
    }
    for (int y = 0; y < DOT_SIZE; y++)
    {
        for (int x = 0; x < DOT_SIZE; x++)
        {
            if (points[y][x])
            {
                RowBits X = 0x01;
                X = X << (DOT_SIZE - 1) - x;
                Frame[y] |= X;
            }
        }
    }
}

void RenderingFrame(RowBits *Frames)
{
    // for y = 0 to 7, write dot matrix device file to Frame[y]
    for (int i = 0; i < DOT_SIZE; i++)
    {
        showBinary(Frames[i]);
    }
}

void showBinary(RowBits binary)
{
    RowBits bin = binary;
    for (int i = 0; i < DOT_SIZE; i++)
    {
        if (bin & 0x80)
        {
            printf("*");
        }
        else
        {
            printf("_");
        }
        bin = bin << 1;
    }
    printf("\n");
}