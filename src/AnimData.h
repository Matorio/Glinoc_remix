#ifndef ANIM_DATA
#define ANIM_DATA

#include <raylib.h>

typedef struct AnimData
{
    Texture2D texture;
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runntingTime;
} AnimData;

AnimData UpdateAnimData(AnimData, float, int);
void UpdateAnimDataArray(AnimData* array, int length, float deltaTime, int maxFrame);

#endif