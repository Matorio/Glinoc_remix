#include "stdbool.h"
#include "AnimData.h"
#include <raylib.h>

// From "main.c"
extern const int windowWidth;
// From "main.c"
extern const int windowHeight;
// From "GameplayScene.c"
extern float ObjectSpeed;

static AnimData* laevas;
static Texture2D laevaTexture;
static const int laevasAmount = 6;

void LoadLaevas(void)
{
    laevaTexture = LoadTexture("textures/LAEVA.png");
    int laevaDist = 100;
    laevas = (AnimData*)MemAlloc(sizeof(AnimData) * laevasAmount);
    for (int i = 0; i < laevasAmount; i++)
    {
        laevas[i].rec.x = 0.0;
        laevas[i].rec.y = 0.0;
        laevas[i].rec.width = laevaTexture.width/4;
        laevas[i].rec.height = laevaTexture.height;
        laevas[i].pos.x = windowWidth + laevaDist;
        laevas[i].pos.y = (windowHeight - 100) - laevaTexture.height;
        laevas[i].frame = 0.0;
        laevas[i].runntingTime = 0.0;
        laevas[i].updateTime = 0.2;
        laevaDist += 10000;
    }
}

void UpdateLaevas(float dt)
{
    UpdateAnimDataArray(laevas, laevasAmount, dt, 3);

    for (int i = 0; i < laevasAmount; i++)
    {        
        if (laevas[i].pos.x <= 0)
        {
            laevas[i].pos.x = windowWidth + i * GetRandomValue(100,500);                        
        }
        else
        {
            laevas[i].pos.x -= (ObjectSpeed) * dt;
        }
    }
}

bool CheckLaevaCollision(AnimData dinoAnimData)
{
    for (int i = 0; i < laevasAmount; i++)
    {
        AnimData Laeva = laevas[i];
        float Laevpad = 30;
        Rectangle LaevRec = 
        {
            Laeva.pos.x + Laevpad,
            Laeva.pos.y + Laevpad,
            Laeva.rec.width - 2*Laevpad,
            Laeva.rec.height - 2*Laevpad
        };
        float DinoPad = 30;
        Rectangle dinoRec =
        {
            dinoAnimData.pos.x + DinoPad,
            dinoAnimData.pos.y + DinoPad,
            dinoAnimData.rec.width - 2*DinoPad,
            dinoAnimData.rec.height - 2*DinoPad
        };
                    
        if (CheckCollisionRecs(LaevRec, dinoRec))
        {
            return true;            
        }            
    }
    return false;
}

void DrawLaevas(void)
{
    for (int i = 0; i < laevasAmount; i++)
    {
        DrawTextureRec(laevaTexture, laevas[i].rec, laevas[i].pos, WHITE);
    }
}

void ResetLaveas(void)
{
    int LaevaDist = 100;
    for (int i = 0; i < laevasAmount; i++)
    {
        laevas[i].rec.x = 0.0;
        laevas[i].rec.y = 0.0;
        laevas[i].rec.width = laevaTexture.width/4;
        laevas[i].rec.height = laevaTexture.height;
        laevas[i].pos.x = windowWidth + LaevaDist;
        laevas[i].pos.y = (windowHeight - 100) - laevaTexture.height;
        laevas[i].frame = 0.0;
        laevas[i].runntingTime = 0.0;
        laevas[i].updateTime = 0.2;
        LaevaDist += 10000;
    }
}

void UnloadLaevas(void)
{
    UnloadTexture(laevaTexture);
    MemFree(laevas);
}