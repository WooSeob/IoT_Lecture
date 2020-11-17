#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Dot_graphic_library.h"


Point *MappingLineToDotMatrix(RowBits *Frames, Line *line);
void Generate_Hex_Code(RowBits *Frame, Point *points, int len);
void RenderingFrame(RowBits* Frames);
void showBinary(RowBits binary);

// To Abstraction Application <-> 8x8 Dot Matrix
void DOT_Draw_Line(Line *line)
{
    RowBits Frames[DOT_SIZE];
    MappingLineToDotMatrix(Frames, line);
    RenderingFrame(Frames);
}

// <--------------------- Private Functions --------------------->
// 1. Mapping
// 2. Generate Hex code
// 3. Rendering with Hex code

int isJoongBock(Point * points, Point target){
    for(int i=0; i<64; i++){
        if(points[i].X == target.X && points[i].Y == target.Y){
            return 1;
        }
    }
    return 0;
}

Point *MappingLineToDotMatrix(RowBits *Frames, Line *line)
{
    Point MappedPoints[64];

    float D = (float)(line->stopY - line->startY) / (float)(line->stopX - line->startX);
    float C = (line->startY) -D*(line->startX);
    //y = Dx + C

    //printf("D: %f, C: %f\n", D, C);

    float rD = (float)(line->stopX - line->startX) / (float)(line->stopY - line->startY);
    float rC = (line->startX) -rD*(line->startY);
    //printf("rD: %f, rC: %f\n", rD, rC);
    //x = rDy + rC

    int len = 0;
    int lStartX = line->startX, lStopX = line->stopX;
    if(lStartX > lStopX){
        int temp = lStartX;
        lStartX = lStopX;
        lStopX = temp;
    }
    //printf("start mapping by x\n");
    for(int x = lStartX; x <= lStopX; x++){
        int y = D*x + C;
        Point coord = {.X = x, .Y = y};
        //printf("mapped (%d, %d)\n", x, y);

        if(!isJoongBock(MappedPoints, coord)){
            MappedPoints[len++] = coord;
            //printf("added\n");
        }
    }
    

    int lStartY = line->startY, lStopY = line->stopY;
    if(lStartY > lStopY){
        int temp = lStartY;
        lStartY = lStopY;
        lStopY = temp;
    }
    //printf("start mapping by y\n");
    for(int y = lStartY; y <= lStopY; y++){
        int x = rD*y + rC;
        Point coord = {.X = x, .Y = y};
        //printf("mapped (%d, %d)\n", x, y);
        
        if(!isJoongBock(MappedPoints, coord)){
            MappedPoints[len++] = coord;
            //printf("added\n");
        }
    }
    
    //printf("convert list to array\n");

    Generate_Hex_Code(Frames, MappedPoints, len);
    //
}

void Generate_Hex_Code(RowBits *Frame, Point *points, int len)
{
    for (int i = 0; i < DOT_SIZE; i++)
    {
        Frame[i] = 0x00;
    }

    for (int i = 0; i < len; i++)
    {
        CoordValue currentY = points[i].Y;
        RowBits X = 0x01;
        X = X << (DOT_SIZE - 1) - points[i].X;
        Frame[currentY] = Frame[currentY] | X;
    }
}

void RenderingFrame(RowBits* Frames)
{
    // for y = 0 to 7, write dot matrix device file to Frame[y]
    for(int i = 0; i < DOT_SIZE; i++){
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