#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "Dot_graphic_library.h"
#include "Lib.h"

#define TEST_SIZE 5

int main(void)
{
    init();

    RenderQueue renderQueue;
    Initialize(&renderQueue);

    Line line1 = {.start = {.X = 5, .Y = 1}, .stop = {.X = 2, .Y = 4}};
    Drawable dLine = {.type = TYPE_Line, .line = &line1};

    Circle circle1 = {.center = {.X = 3, .Y = 4}, .radius = 2};
    Drawable dCircle = {.type = TYPE_Circle, .circle = &circle1};

    Line line2 = {.start = {.X = 1, .Y = 1}, .stop = {.X = 4, .Y = 4}};
    Drawable dLine2 = {.type = TYPE_Line, .line = &line2};

    // Point points[3] = {{.X = 3, .Y = 3}, {.X = 2, .Y = 2}, {.X = 4, .Y = 5}};
    // Points pointGroup = {.points = points, .len = 3};
    // SetPoints(&pointGroup);
    // Drawable dPoints = {.type = TYPE_Points, .Points = &pointGroup};

    Point points[1] = {{.X = 4, .Y = 4}};
    Points pointGroup = {.points = points, .len = 1};
    SetPoints(&pointGroup);
    Drawable dPointOnlyOne = {.type = TYPE_Points, .Points = &pointGroup};

    // InsertFront(&renderQueue, &dPoints);
    // InsertFront(&renderQueue, &dLine);
    // InsertFront(&renderQueue, &dCircle);
    // InsertFront(&renderQueue, &dLine2);
    InsertFront(&renderQueue, &dPointOnlyOne);


    int dX = 1;
    int dY = 1;
    while(1){
        DOT_Draw_Canvas(&renderQueue);
        
        if(isX_Out(pointGroup.right_bottom.X) || isX_Out(pointGroup.left_top.X)){
            dX = -dX;
        }
        if(isY_Out(pointGroup.right_bottom.Y) || isY_Out(pointGroup.left_top.Y)){
            dY = -dY;
        }
        MovePoints(&pointGroup, dX, dY);

        
        usleep(160000);
    }

    Clear(&renderQueue);
    return 0;
}
