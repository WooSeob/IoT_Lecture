#include "emoticon.h"

#define TEST_SIZE 5

Point SUNNY_1[60] = {{.X = 0, .Y = 1}, {.X = 0, .Y = 2}, {.X = 0, .Y = 3}, {.X = 0, .Y = 4}, {.X = 0, .Y = 5}, {.X = 0, .Y = 6}, {.X = 1, .Y = 0}, {.X = 1, .Y = 1}, {.X = 1, .Y = 2}, {.X = 1, .Y = 3}, {.X = 1, .Y = 4}, {.X = 1, .Y = 5}, {.X = 1, .Y = 6}, {.X = 1, .Y = 7}, {.X = 2, .Y = 0}, {.X = 2, .Y = 1}, {.X = 2, .Y = 2}, {.X = 2, .Y = 3}, {.X = 2, .Y = 4}, {.X = 2, .Y = 5}, {.X = 2, .Y = 6}, {.X = 2, .Y = 7}, {.X = 3, .Y = 0}, {.X = 3, .Y = 1}, {.X = 3, .Y = 2}, {.X = 3, .Y = 3}, {.X = 3, .Y = 4}, {.X = 3, .Y = 5}, {.X = 3, .Y = 6}, {.X = 3, .Y = 7}, {.X = 4, .Y = 0}, {.X = 4, .Y = 1}, {.X = 4, .Y = 2}, {.X = 4, .Y = 3}, {.X = 4, .Y = 4}, {.X = 4, .Y = 5}, {.X = 4, .Y = 6}, {.X = 4, .Y = 7}, {.X = 5, .Y = 0}, {.X = 5, .Y = 1}, {.X = 5, .Y = 2}, {.X = 5, .Y = 3}, {.X = 5, .Y = 4}, {.X = 5, .Y = 5}, {.X = 5, .Y = 6}, {.X = 5, .Y = 7}, {.X = 6, .Y = 0}, {.X = 6, .Y = 1}, {.X = 6, .Y = 2}, {.X = 6, .Y = 3}, {.X = 6, .Y = 4}, {.X = 6, .Y = 5}, {.X = 6, .Y = 6}, {.X = 6, .Y = 7}, {.X = 7, .Y = 1}, {.X = 7, .Y = 2}, {.X = 7, .Y = 3}, {.X = 7, .Y = 4}, {.X = 7, .Y = 5}, {.X = 7, .Y = 6}};
//맑음2
Point SUNNY_2[32] = {{.X = 1, .Y = 2}, {.X = 1, .Y = 3}, {.X = 1, .Y = 4}, {.X = 1, .Y = 5}, {.X = 2, .Y = 1}, {.X = 2, .Y = 2}, {.X = 2, .Y = 3}, {.X = 2, .Y = 4}, {.X = 2, .Y = 5}, {.X = 2, .Y = 6}, {.X = 3, .Y = 1}, {.X = 3, .Y = 2}, {.X = 3, .Y = 3}, {.X = 3, .Y = 4}, {.X = 3, .Y = 5}, {.X = 3, .Y = 6}, {.X = 4, .Y = 1}, {.X = 4, .Y = 2}, {.X = 4, .Y = 3}, {.X = 4, .Y = 4}, {.X = 4, .Y = 5}, {.X = 4, .Y = 6}, {.X = 5, .Y = 1}, {.X = 5, .Y = 2}, {.X = 5, .Y = 3}, {.X = 5, .Y = 4}, {.X = 5, .Y = 5}, {.X = 5, .Y = 6}, {.X = 6, .Y = 2}, {.X = 6, .Y = 3}, {.X = 6, .Y = 4}, {.X = 6, .Y = 5}};

//구름많음1
Point CLOUDY_1[19] = {{.X = 0, .Y = 0}, {.X = 0, .Y = 1}, {.X = 0, .Y = 6}, {.X = 1, .Y = 0}, {.X = 1, .Y = 5}, {.X = 2, .Y = 4}, {.X = 2, .Y = 5}, {.X = 3, .Y = 5}, {.X = 3, .Y = 6}, {.X = 4, .Y = 6}, {.X = 4, .Y = 7}, {.X = 4, .Y = 1}, {.X = 4, .Y = 2}, {.X = 5, .Y = 0}, {.X = 5, .Y = 1}, {.X = 5, .Y = 2}, {.X = 5, .Y = 3}, {.X = 6, .Y = 1}, {.X = 6, .Y = 2}};
//구름많음2
Point CLOUDY_2[18] = {{.X = 0, .Y = 0}, {.X = 0, .Y = 1}, {.X = 0, .Y = 2}, {.X = 0, .Y = 3}, {.X = 1, .Y = 1}, {.X = 1, .Y = 2}, {.X = 1, .Y = 7}, {.X = 2, .Y = 6}, {.X = 2, .Y = 7}, {.X = 3, .Y = 7}, {.X = 4, .Y = 3}, {.X = 4, .Y = 4}, {.X = 5, .Y = 2}, {.X = 5, .Y = 3}, {.X = 5, .Y = 4}, {.X = 5, .Y = 5}, {.X = 6, .Y = 3}, {.X = 6, .Y = 4}};

//비1
Point RAIN_1[20] = {{.X = 0, .Y = 0}, {.X = 0, .Y = 1}, {.X = 0, .Y = 4}, {.X = 0, .Y = 5}, {.X = 0, .Y = 6}, {.X = 2, .Y = 2}, {.X = 2, .Y = 3}, {.X = 2, .Y = 5}, {.X = 2, .Y = 6}, {.X = 2, .Y = 7}, {.X = 4, .Y = 0}, {.X = 4, .Y = 1}, {.X = 4, .Y = 4}, {.X = 4, .Y = 5}, {.X = 4, .Y = 6}, {.X = 6, .Y = 2}, {.X = 6, .Y = 3}, {.X = 6, .Y = 5}, {.X = 6, .Y = 6}, {.X = 6, .Y = 7}};

//비2
Point RAIN_2[20] = {{.X = 1, .Y = 0}, {.X = 1, .Y = 1}, {.X = 1, .Y = 4}, {.X = 1, .Y = 5}, {.X = 1, .Y = 6}, {.X = 3, .Y = 2}, {.X = 3, .Y = 3}, {.X = 3, .Y = 5}, {.X = 3, .Y = 6}, {.X = 3, .Y = 7}, {.X = 5, .Y = 0}, {.X = 5, .Y = 1}, {.X = 5, .Y = 4}, {.X = 5, .Y = 5}, {.X = 5, .Y = 6}, {.X = 7, .Y = 2}, {.X = 7, .Y = 3}, {.X = 7, .Y = 5}, {.X = 7, .Y = 6}, {.X = 7, .Y = 7}};

//비&눈1
Point RAIN_SNOW_1[19] = {{.X = 0, .Y = 0}, {.X = 0, .Y = 1}, {.X = 0, .Y = 4}, {.X = 0, .Y = 5}, {.X = 0, .Y = 6}, {.X = 2, .Y = 3}, {.X = 2, .Y = 6}, {.X = 2, .Y = 7}, {.X = 3, .Y = 2}, {.X = 3, .Y = 3}, {.X = 3, .Y = 4}, {.X = 4, .Y = 0}, {.X = 4, .Y = 1}, {.X = 4, .Y = 3}, {.X = 6, .Y = 2}, {.X = 6, .Y = 3}, {.X = 6, .Y = 5}, {.X = 6, .Y = 6}, {.X = 6, .Y = 7}};

//비&눈2

Point RAIN_SNOW_2[19] = {{.X = 1, .Y = 0}, {.X = 1, .Y = 1}, {.X = 1, .Y = 4}, {.X = 1, .Y = 5}, {.X = 1, .Y = 6}, {.X = 3, .Y = 4}, {.X = 3, .Y = 6}, {.X = 3, .Y = 7}, {.X = 4, .Y = 3}, {.X = 4, .Y = 4}, {.X = 4, .Y = 5}, {.X = 5, .Y = 0}, {.X = 5, .Y = 1}, {.X = 5, .Y = 4}, {.X = 7, .Y = 2}, {.X = 7, .Y = 3}, {.X = 7, .Y = 5}, {.X = 7, .Y = 6}, {.X = 7, .Y = 7}};

//눈1
Point SNOW_1[15] = {{.X = 0, .Y = 1},
                    {.X = 1, .Y = 0},
                    {.X = 1, .Y = 1},
                    {.X = 1, .Y = 2},
                    {.X = 2, .Y = 1},
                    {.X = 2, .Y = 6},
                    {.X = 3, .Y = 5},
                    {.X = 3, .Y = 6},
                    {.X = 3, .Y = 7},
                    {.X = 4, .Y = 6},
                    {.X = 5, .Y = 2},
                    {.X = 6, .Y = 1},
                    {.X = 6, .Y = 2},
                    {.X = 6, .Y = 3},
                    {.X = 7, .Y = 2}};

//눈2
Point SNOW_2[14] = {
    {.X = 1, .Y = 2},
    {.X = 2, .Y = 1},
    {.X = 2, .Y = 2},
    {.X = 2, .Y = 3},
    {.X = 3, .Y = 2},
    {.X = 3, .Y = 7},
    {.X = 4, .Y = 6},
    {.X = 4, .Y = 7},
    {.X = 5, .Y = 4},
    {.X = 5, .Y = 7},
    {.X = 6, .Y = 3},
    {.X = 6, .Y = 4},
    {.X = 6, .Y = 5},
    {.X = 7, .Y = 4},
};

Point T[12] = {{.X = 1, .Y = 1},
               {.X = 2, .Y = 1},
               {.X = 3, .Y = 1},
               {.X = 4, .Y = 1},
               {.X = 5, .Y = 1},
               {.X = 6, .Y = 1},
               {.X = 4, .Y = 1},
               {.X = 4, .Y = 2},
               {.X = 4, .Y = 3},
               {.X = 4, .Y = 4},
               {.X = 4, .Y = 5},
               {.X = 4, .Y = 6}};

Point I[10] = {{.X = 5, .Y = 1},
              {.X = 5, .Y = 2},
              {.X = 5, .Y = 3},
              {.X = 5, .Y = 4},
              {.X = 5, .Y = 5},
              {.X = 5, .Y = 6},
              {.X = 4, .Y = 1},
              {.X = 6, .Y = 1},
              {.X = 4, .Y = 6},
              {.X = 6, .Y = 6},
              };

Point M[17] = {{.X = 1, .Y = 1},
               {.X = 1, .Y = 2},
               {.X = 1, .Y = 3},
               {.X = 1, .Y = 4},
               {.X = 1, .Y = 5},
               {.X = 1, .Y = 6},
               {.X = 7, .Y = 1},
               {.X = 7, .Y = 2},
               {.X = 7, .Y = 3},
               {.X = 7, .Y = 4},
               {.X = 7, .Y = 5},
               {.X = 7, .Y = 6},
               {.X = 2, .Y = 2},
               {.X = 3, .Y = 3},
               {.X = 4, .Y = 4},
               {.X = 5, .Y = 3},
               {.X = 6, .Y = 2}};

Point E1[15] = {{.X = 2, .Y = 1},
               {.X = 3, .Y = 1},
               {.X = 4, .Y = 1},
               {.X = 5, .Y = 1},
               {.X = 2, .Y = 3},
               {.X = 3, .Y = 3},
               {.X = 4, .Y = 3},
               {.X = 5, .Y = 3},
               {.X = 2, .Y = 6},
               {.X = 3, .Y = 6},
               {.X = 4, .Y = 6},
               {.X = 5, .Y = 6},
               {.X = 2, .Y = 2},
               {.X = 2, .Y = 4},
               {.X = 2, .Y = 5}};

Point O[12] = {{.X = 3, .Y = 1},
               {.X = 4, .Y = 1},
               {.X = 3, .Y = 6},
               {.X = 4, .Y = 6},
               {.X = 2, .Y = 2},
               {.X = 2, .Y = 3},
               {.X = 2, .Y = 4},
               {.X = 2, .Y = 5},
               {.X = 5, .Y = 2},
               {.X = 5, .Y = 3},
               {.X = 5, .Y = 4},
               {.X = 5, .Y = 5}};

Point V[10] = {{.X = 2, .Y = 1},
               {.X = 2, .Y = 2},
               {.X = 3, .Y = 3},
               {.X = 3, .Y = 4},
               {.X = 4, .Y = 5},
               {.X = 4, .Y = 6},
               {.X = 5, .Y = 4},
               {.X = 5, .Y = 3},
               {.X = 6, .Y = 2},
               {.X = 6, .Y = 1}};

Point E2[15] = {{.X = 2, .Y = 1},
               {.X = 3, .Y = 1},
               {.X = 4, .Y = 1},
               {.X = 5, .Y = 1},
               {.X = 2, .Y = 3},
               {.X = 3, .Y = 3},
               {.X = 4, .Y = 3},
               {.X = 5, .Y = 3},
               {.X = 2, .Y = 6},
               {.X = 3, .Y = 6},
               {.X = 4, .Y = 6},
               {.X = 5, .Y = 6},
               {.X = 2, .Y = 2},
               {.X = 2, .Y = 4},
               {.X = 2, .Y = 5}};

Point R[14] = {{.X = 2, .Y = 1},
               {.X = 2, .Y = 2},
               {.X = 2, .Y = 3},
               {.X = 2, .Y = 4},
               {.X = 2, .Y = 5},
               {.X = 2, .Y = 6},
               {.X = 3, .Y = 1},
               {.X = 4, .Y = 1},
               {.X = 3, .Y = 3},
               {.X = 3, .Y = 3},
               {.X = 5, .Y = 5},
               {.X = 5, .Y = 6},
               {.X = 5, .Y = 2},
               {.X = 4, .Y = 4}};


Points alphabet_T = {.points = T, .len = 12};
Points alphabet_I = {.points = I, .len = 10};
Points alphabet_M = {.points = M, .len = 17};
Points alphabet_E1 = {.points = E1, .len = 15};
Points alphabet_O = {.points = O, .len = 12};
Points alphabet_V = {.points = V, .len = 10};
Points alphabet_E2 = {.points = E2, .len = 15};
Points alphabet_R = {.points = R, .len = 14};

Points EMOTICONS[10] = {{.points = SUNNY_1, .len = 60}, {.points = SUNNY_2, .len = 32}, {.points = CLOUDY_1, .len = 19}, {.points = CLOUDY_2, .len = 18}, {.points = RAIN_1, .len = 20}, {.points = RAIN_2, .len = 20}, {.points = RAIN_SNOW_1, .len = 19}, {.points = RAIN_SNOW_2, .len = 19}, {.points = SNOW_1, .len = 15}, {.points = SNOW_2, .len = 14}};
int AnimationFrame = 0;

void playEmoticon(int Weather)
{
    int EmoticonOffset = (Weather - 1) * 2;
    Drawable dFrame_1 = {.type = TYPE_Points, .Points = &EMOTICONS[EmoticonOffset]};
    Drawable dFrame_2 = {.type = TYPE_Points, .Points = &EMOTICONS[EmoticonOffset + 1]};

    RenderQueue Frame1, Frame2;

    Initialize(&Frame1);
    Initialize(&Frame2);

    InsertFront(&Frame1, &dFrame_1);
    InsertFront(&Frame2, &dFrame_2);

    if (AnimationFrame % 2 == 0)
    {
        DOT_Draw_Canvas(&Frame1);
    }
    else
    {
        DOT_Draw_Canvas(&Frame2);
    }
    AnimationFrame++;
}

void playTimeover(int FrameCnt)
{
    int i;
    Drawable chars[8] = {{.type = TYPE_Points, .Points = &alphabet_T},
                         {.type = TYPE_Points, .Points = &alphabet_I},
                         {.type = TYPE_Points, .Points = &alphabet_M},
                         {.type = TYPE_Points, .Points = &alphabet_E1},
                         {.type = TYPE_Points, .Points = &alphabet_O},
                         {.type = TYPE_Points, .Points = &alphabet_V},
                         {.type = TYPE_Points, .Points = &alphabet_E2},
                         {.type = TYPE_Points, .Points = &alphabet_R}};
    
    RenderQueue Frame;
    Initialize(&Frame);

    if(FrameCnt==0){
        printf("emoticon.c : playTimeover > init points \n");
        int i;
        for(i = 0; i < 8; i++){
            SetPoints(chars[i].Points);
            SetPointsByLeftTop(chars[i].Points, 8*i + 3, 1);
        }
    }else{
        for(i = 0; i < 8; i++){
            MovePoints(chars[i].Points, -1, 0);
        }
    }
    
    for(i = 0; i < 8; i++){
        InsertFront(&Frame, &chars[i]);
    }
    // InsertFront(&Frame, &chars[2]);

    DOT_Draw_Canvas(&Frame);
}
