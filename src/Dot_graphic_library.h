//Use int As 2D Coordinate system value


typedef int CoordValue;
typedef unsigned char RowBits;

typedef struct Point
{
    CoordValue X;
    CoordValue Y;
} Point;

typedef struct Line
{
    Point start;
    Point stop;
} Line;

typedef struct Circle
{
    Point center;
    CoordValue radius;
} Circle;

#define DOT_SIZE 8
#define INIT_POINTS \
    {               \
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
        {0, 0, 0, 0, 0, 0, 0, 0},\
    }\

void DOT_Draw_Points(Point points[], int len);
void DOT_Draw_Circle(Circle *circle);
void DOT_Draw_Line(Line *line);
void DOT_Draw_Lines(Line line[], int len);

// test
void Generate_Hex_Code(RowBits *Frame, char points[][DOT_SIZE]);
void showBinary(RowBits binary);