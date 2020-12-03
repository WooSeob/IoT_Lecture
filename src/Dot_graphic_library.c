#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#define dot_dev "/dev/dot"

#include "Dot_graphic_library.h"
#include "Lib.h"
#include "io.h"

// int dot_fd = 0;

// void init(){
//     dot_fd = open(dot_dev, O_RDWR);
//     if (dot_fd < 0)
//     {
//         printf("Can't Open Device\n");
//     }
// }
void SetPointsByLeftTop(Points *pointGroup, CoordValue X, CoordValue Y){
    int offsetX, offsetY;
    Point lt = pointGroup->left_top;
    offsetX = X - lt.X;
    offsetY = Y - lt.Y;
    MovePoints(pointGroup, offsetX, offsetY);
}

void SetPointsByCenter(Points *pointGroup, CoordValue X, CoordValue Y){
    int offsetX, offsetY;
    Point lt = pointGroup->left_top;
    Point rb = pointGroup->right_bottom;

    int centerX = (lt.X + rb.X)/2;
    int centerY = (lt.Y + rb.Y)/2;
    printf("centerX : %d, centerY : %d To X : %d, Y : %d\n", centerX, centerY, X, Y);

    offsetX = X - centerX;
    offsetY = Y - centerY;
    printf("offsetX : %d, offsetY : %d\n", offsetX, offsetY);
    MovePoints(pointGroup, offsetX, offsetY);

    centerX = (pointGroup->left_top.X + pointGroup->right_bottom.X)/2;
    centerY = (pointGroup->left_top.Y + pointGroup->right_bottom.Y)/2;
    printf("now centerX : %d, centerY : %d\n", centerX, centerY);
}

void MovePoints(Points *pointGroup, CoordValue offsetX, CoordValue offsetY){
    int i;
    Point *P = pointGroup->points;

    for(i = 0; i < pointGroup->len; i++){
        P[i].X += offsetX;
        P[i].Y += offsetY;
    }
    Point lt = pointGroup->left_top;
    lt.X += offsetX;
    lt.Y += offsetY;
    pointGroup->left_top = lt;

    Point rb = pointGroup->right_bottom;
    rb.X += offsetX;
    rb.Y += offsetY;
    pointGroup->right_bottom = rb;
    SetPoints(pointGroup);
}

void SetPoints(Points *pointGroup){
    int i;
    int Xmax = 0, Xmin = DOT_SIZE, Ymax = 0, Ymin = DOT_SIZE;

    Point *P = pointGroup->points;

    for(i = 0; i < pointGroup->len; i++){
        if(Xmax < P[i].X){
            Xmax = P[i].X;
        }
        if(Xmin > P[i].X){
            Xmin = P[i].X;
        }
        if(Ymax < P[i].Y){
            Ymax = P[i].Y;
        }
        if(Ymin > P[i].Y){
            Ymin = P[i].Y;
        }
    }
    
    pointGroup->width = Xmax - Xmin;
    pointGroup->height = Ymax - Ymin;

    Point lt = {.X = Xmin, .Y = Ymin};
    pointGroup->left_top = lt;
    Point rb = {.X = Xmax, .Y = Ymax};
    pointGroup->right_bottom = rb;
}
// To Abstraction Application <-> 8x8 Dot Matrix
void DOT_Draw_Canvas(RenderQueue *renderQueue)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    while (renderQueue->cur != NULL)
    {
        Drawable D = renderQueue->cur->value;

        switch (D.type)
        {
        case TYPE_Points:
            MappingPointsToDotMatrix(MappedPoints, D.Points);
            break;

        case TYPE_Line:
            MappingLineToDotMatrix(MappedPoints, D.line);
            break;

        case TYPE_Circle:
            MappingCircleToDotMatrix(MappedPoints, D.circle);    
            break;

        default:
            printf("Drawble Type invalid! : %d\n", D.type);
            break;
        }
        
        MoveNext(renderQueue);
    }
    //그리기
    Generate_Hex_Code(Frames, MappedPoints);
    RenderingFrame(Frames);
    //커서 맨앞자리로 다시 옮겨놓기
    renderQueue->cur = renderQueue->head;
}

// <--------------------- Private Functions --------------------->
// 1. Mapping
// 2. Generate Hex code
// 3. Rendering with Hex code

int isX_Out(CoordValue x){
    return x < 0 || x >= DOT_SIZE;
}
int isY_Out(CoordValue y){
    return y < 0 || y >= DOT_SIZE;
}
int isInBound(CoordValue x, CoordValue y){
    // return (x > 0 && x < DOT_SIZE && y > 0 && y < DOT_SIZE);
    return !isX_Out(x) && !isY_Out(y);
}

void addMappedPoint(char MappedPoints[][DOT_SIZE], CoordValue x, CoordValue y){
    if(isInBound(x, y)){
        MappedPoints[y][x] = 1;
    }
}

void MappingPointsToDotMatrix(char MappedPoints[][DOT_SIZE], Points* pointGroup){
    int i, len = pointGroup->len;
    
    for(i=0; i<len; i++){
        Point p = pointGroup->points[i];
        addMappedPoint(MappedPoints, p.X, p.Y);
    }
}

void MappingLineToDotMatrix(char MappedPoints[][DOT_SIZE], Line *line)
{
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
    int x;
    for (x = lStartX; x <= lStopX; x++)
    {
        int y = D * x + C;
        addMappedPoint(MappedPoints, x, y);
    }

    //# y기준 매핑
    int lStartY = start.Y > stop.Y ? stop.Y : start.Y;
    int lStopY = start.Y > stop.Y ? start.Y : stop.Y;
    int y;
    for (y = lStartY; y <= lStopY; y++)
    {
        int x = rD * y + rC;
        addMappedPoint(MappedPoints, x, y);
    }
}

void MappingCircleToDotMatrix(char MappedPoints[][DOT_SIZE], Circle *circle)
{
    CoordValue R = circle->radius;
    Point Center = circle->center;

    //# x기준 매핑
    int lStartX = Center.X - R;
    int lStopX = Center.X + R;
    int x;
    for (x = lStartX; x <= lStopX; x++)
    {
        int y1 = sqrt(pow(R, 2) - pow((x - Center.X), 2)) + Center.Y;
        int y2 = -sqrt(pow(R, 2) - pow((x - Center.X), 2)) + Center.Y;
        addMappedPoint(MappedPoints, x, y1);
        addMappedPoint(MappedPoints, x, y2);
        
    }

    //# y기준 매핑
    int lStartY = Center.Y - R;
    int lStopY = Center.Y + R;
    int y;
    for (y = lStartY; y <= lStopY; y++)
    {
        int x1 = sqrt(pow(R, 2) - pow((y - Center.Y), 2)) + Center.X;
        int x2 = -sqrt(pow(R, 2) - pow((y - Center.Y), 2)) + Center.X;
        addMappedPoint(MappedPoints, x1, y);
        addMappedPoint(MappedPoints, x2, y);
    }
}

void Generate_Hex_Code(RowBits *Frame, char points[][DOT_SIZE])
{
    int i;
    for (i = 0; i < DOT_SIZE; i++)
    {
        Frame[i] = 0x00;
    }
    int y;
    for (y = 0; y < DOT_SIZE; y++)
    {
        int x;
        for (x = 0; x < DOT_SIZE; x++)
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
    WriteToDot(Frames); //
    WriteToConsole(Frames);
}

void showBinary(RowBits binary)
{
    RowBits bin = binary;
    int i;
    for (i = 0; i < DOT_SIZE; i++)
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

void WriteToConsole(RowBits *Frame){
    int i;
    for(i = 0; i< DOT_SIZE; i++){
        showBinary(Frame[i]);
    }
    printf("\n");
}

void WriteToDot(RowBits *Frame)
{
    PrintToDOT(Frame);
}
