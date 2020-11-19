//Use int As 2D Coordinate system value


typedef int CoordValue;
typedef unsigned char RowBits;

typedef struct Point
{
    CoordValue X;
    CoordValue Y;
} Point;

typedef struct Points
{
    Point *points;
    char len;

    Point left_top;
    CoordValue width;
    CoordValue height;
} Points;

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

typedef struct Drawable
{
    char type;
    Point *Points;
    Line *line;
    Circle *circle;
} Drawable;

typedef struct Node{
    struct Node *next;
    Drawable value;
} Node;

typedef struct RenderQueue{
    Node* head;
    Node* cur;
} RenderQueue;

#define TYPE_Points 1
#define TYPE_Line 2
#define TYPE_Circle 3

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
void DOT_Draw_Canvas(RenderQueue *renderQueue);

// test
void Generate_Hex_Code(RowBits *Frame, char points[][DOT_SIZE]);
void showBinary(RowBits binary);

void MappingLineToDotMatrix(char MappedPoints[][DOT_SIZE], Line *line);
void MappingCircleToDotMatrix(char MappedPoints[][DOT_SIZE], Circle *circle);
void RenderingFrame(RowBits *Frames);

void WriteToConsole(RowBits *Frame);
void WriteToDot(RowBits *Frame);

void MoveNext (RenderQueue* list);
void InsertFront (RenderQueue* list, const Drawable* x);
void Initialize (RenderQueue* list); 
void Clear (RenderQueue* list); 