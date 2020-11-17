#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Dot_graphic_library.h"

int partition(Point *A, int p, int r)
{
    Point pivot = A[r];
    int i = p - 1;

    Point temp;
    for (int j = p; j < r; j++)
    {
        if (A[j].Y <= pivot.Y)
        {
            temp = A[j];
            A[j] = A[++i];
            A[i] = temp;
        }
    }

    temp = A[r];
    A[r] = A[i + 1];
    A[i + 1] = temp;

    return i + 1;
}

void Sort(Point *A, int p, int r)
{
    if (p < r)
    {
        int q = partition(A, p, r);
        Sort(A, p, q - 1);
        Sort(A, q + 1, r);
    }
}

// To Abstraction Application <-> 8x8 Dot Matrix
void DOT_Draw_Line(Line *line)
{
    //
}

// <--------------------- Private Functions --------------------->
// 1. Mapping
// 2. Generate Hex code
// 3. Rendering with Hex code

Point *MappingLineToDotMatrix(Line *line)
{
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

void RenderingFrame()
{
    // for y = 0 to 7, write dot matrix device file to Frame[y]
}

void showBinary(RowBits binary){
    RowBits bin = binary;
    for(int i = 0; i < DOT_SIZE; i++){
        if(bin & 0x80){
            printf("*");
        }else{
            printf("_");
        }
        bin = bin << 1;
    }
    printf("\n");
}