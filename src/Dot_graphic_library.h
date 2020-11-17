//Use int As 2D Coordinate system value
typedef int CoordValue;
typedef unsigned char RowBits;

typedef struct Line{
    CoordValue startX;
    CoordValue startY;
    CoordValue stopX;
    CoordValue stopY;
} Line;

typedef struct Point{
    CoordValue X;
    CoordValue Y;
} Point;

#define DOT_SIZE 8

void DOT_Draw_Line(Line * line);

// test
void Generate_Hex_Code(RowBits* Frame, Point *points, int len);

void showBinary(RowBits binary);