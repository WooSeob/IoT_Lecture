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

// To Abstraction Application <-> 8x8 Dot Matrix
void DOT_Draw_Canvas(RenderQueue *renderQueue)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];

    while (renderQueue->cur != NULL)
    {
        Drawable D = renderQueue->cur->value;
        printf("draw %d\n", D.type);

        switch (D.type)
        {
        case TYPE_Points:
            // int i = 0;
            // for (i = 0; i < ; i++)
            // {
            //     Point p = D.Points[i];
            //     MappedPoints[p.Y][p.X] = 1;
            // }
            break;

        case TYPE_Line:
            MappingLineToDotMatrix(MappedPoints, D.line);
            break;

        case TYPE_Circle:
            /* code */
            MappingCircleToDotMatrix(MappedPoints, D.circle);    
            break;

        default:
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
void DOT_Draw_Points(Point points[], int len)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];

    int i = 0;
    for (i = 0; i < len; i++)
    {
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

    MappingCircleToDotMatrix(MappedPoints, circle);
    Generate_Hex_Code(Frames, MappedPoints);

    RenderingFrame(Frames);
}

void DOT_Draw_Line(Line *line)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];

    MappingLineToDotMatrix(MappedPoints, line);
    Generate_Hex_Code(Frames, MappedPoints);

    RenderingFrame(Frames);
}

void DOT_Draw_Lines(Line line[], int len)
{
    char MappedPoints[DOT_SIZE][DOT_SIZE] = INIT_POINTS;
    RowBits Frames[DOT_SIZE];
    int i = 0;
    for (i = 0; i < len; i++)
    {
        MappingLineToDotMatrix(MappedPoints, &line[i]);
    }
    Generate_Hex_Code(Frames, MappedPoints);

    RenderingFrame(Frames);
}

// <--------------------- Private Functions --------------------->
// 1. Mapping
// 2. Generate Hex code
// 3. Rendering with Hex code

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
        MappedPoints[y][x] = 1;
    }

    //# y기준 매핑
    int lStartY = start.Y > stop.Y ? stop.Y : start.Y;
    int lStopY = start.Y > stop.Y ? start.Y : stop.Y;
    int y;
    for (y = lStartY; y <= lStopY; y++)
    {
        int x = rD * y + rC;
        MappedPoints[y][x] = 1;
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
        MappedPoints[y1][x] = 1;
        MappedPoints[y2][x] = 1;
    }

    //# y기준 매핑
    int lStartY = Center.Y - R;
    int lStopY = Center.Y + R;
    int y;
    for (y = lStartY; y <= lStopY; y++)
    {
        int x1 = sqrt(pow(R, 2) - pow((y - Center.Y), 2)) + Center.X;
        int x2 = -sqrt(pow(R, 2) - pow((y - Center.Y), 2)) + Center.X;
        MappedPoints[y][x1] = 1;
        MappedPoints[y][x2] = 1;
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
    int dot_fd = 0;
    dot_fd = open(dot_dev, O_RDWR);

    if (dot_fd < 0)
    {
        printf("Can't Open Device\n");
    }
    write(dot_fd, Frame, DOT_SIZE);
}


static Node* AllocNode() {
	return calloc(1, sizeof(Node));
}

//n이 가리키는 노드의 각 멤버에 값을 설정
static void SetNode(Node* n, const Drawable* x, const Node* next) {
	n->value = *x;
    n->next = next;
}

//초기화
void Initialize (RenderQueue* list) {
	list->head = NULL;
    list->cur = NULL;
}

//머리에 노드 삽입
void InsertFront (RenderQueue* list, const Drawable* x) {
	Node* ptr = list->head;
    list->head = list->cur = AllocNode();
    SetNode(list->head, x, ptr);
}

//머리 노드를 삭제
void RemoveFront (RenderQueue* list) {
	if(list->head != NULL) {
    	Node* ptr = list->head->next;
        free(list->head);
        list->head = list->cur = ptr;
    }
}

void MoveNext (RenderQueue* list){
    Node * n = list->cur;
    if(n != NULL){
        n = n->next;
        list->cur = n;
    }
}

//모든 노드를 삭제
void Clear (RenderQueue* list) {
	while(list->head != NULL)
    	RemoveFront(list);
    list->cur = NULL;
}

