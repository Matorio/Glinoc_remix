#include <raylib.h>
#include <stdbool.h>
#include "AnimData.h"
#include "GameplayScene.h"
#include "Laevas.h"
#include "Environment.h"
#include "Dino.h"
#include "ControlDraw.h"

// From "main.c"
extern bool musicOn;
// From "main.c"
extern const int windowWidth;
// From "main.c"
extern const int windowHeight;
// From "main.c"
extern AnimData dinoAnimData;

//Object Speed
float ObjectSpeed = 300;
//points
static int pointNum = 0;
static float pointRunningTime = 0;
static float pointUpdateTime = 1;

static AnimData* melons;
static Texture2D melonTexture;
static const int melonsAmount = 1;
static int melonDist;

static bool alive = true;

static Sound JumpSound;
static Sound melonPickUpSound;
static Sound killSound;
static Music PixelKingMusic;

static void LoadMelons(void);
static void DrawAliveScene(void);
static void DrawDeadScene(void);

void StartGameplayScene(void)
{
    TraceLog(LOG_INFO, "Loading gameplay scene...");
    LoadLaevas();
    LoadMelons();
    LoadEnvironment();
    LoadDino();
    
    melonPickUpSound = LoadSound("Sounds/pickupCoin.wav");
    killSound = LoadSound("Sounds/hitHurt.wav");
    PixelKingMusic = LoadMusicStream("Sounds/Pixel-Kings.mp3");
    PixelKingMusic.looping = true;
    PlayMusicStream(PixelKingMusic);
}

void LoadMelons(void)
{
    melonTexture = LoadTexture("textures/Yellow Watermelon glow.png");
    melonDist = 1000;

    melons = (AnimData*)MemAlloc(sizeof(AnimData) * melonsAmount);
    for (int i = 0; i < melonsAmount; i++)
    {
        melons[i].rec.x = 0.0;
        melons[i].rec.y = 0.0;
        melons[i].rec.width = melonTexture.width;
        melons[i].rec.height = melonTexture.height;
        melons[i].pos.x = windowWidth + melonDist;
        melons[i].pos.y = (windowHeight - 200) - melonTexture.height;
        melons[i].frame = 0.0;
        melons[i].runntingTime = 0.0;
        melons[i].updateTime = 0.2;
        melonDist += 2000;
    }
}

void UpdateGameplayScene(void)
{
    if(alive == false)
    {
        return;
    }

    if(CheckLaevaCollision(dinoAnimData) && !DinoIsDuck())
    {
        alive = false;
        PlaySound(killSound);
        StopMusicStream(PixelKingMusic);
    }

    for (int i = 0; i < melonsAmount; i++)
    {
        AnimData Melon = melons[i];
        const float MelonPad = 5;
        const Rectangle MelonRec =
        {
            Melon.pos.x + MelonPad,
            Melon.pos.y + MelonPad,
            Melon.rec.width - 2*MelonPad,
            Melon.rec.height - 2*MelonPad
        };
        const float DinoPad = 30;
        const Rectangle dinoRec =
        {
            dinoAnimData.pos.x + DinoPad,
            dinoAnimData.pos.y + DinoPad,
            dinoAnimData.rec.width - 2*DinoPad,
            dinoAnimData.rec.height - 2*DinoPad
        };
        
        if (CheckCollisionRecs(MelonRec, dinoRec))
        {
            PlaySound(melonPickUpSound);
            pointNum += 10;
            melons[i].pos.x = windowWidth + GetRandomValue(2000,20000);
        }
    }
    
    //Music Logic
    if (musicOn)
    {
        UpdateMusicStream(PixelKingMusic);
    }

    float dt = GetFrameTime();
    UpdateLaevas(dt);
    UpdateEnvironment(dt);

    //Points Health
    pointRunningTime += dt;
    if (pointRunningTime >= pointUpdateTime)
    {   
        pointRunningTime = 0.0;
        pointNum++;
        if (ObjectSpeed < 700)
        {
            IncreaseEnvironmentSpeed();
            ObjectSpeed+=7;
        }    
    }

    UpdateDino();
}

void DrawGameplayScene(void)
{
    if (alive)
    {
        DrawAliveScene();
    }
    else
    {
        DrawDeadScene();
    }
}

void DrawAliveScene(void)
{
    //Delta Time
    float dt = GetFrameTime();

    //Update position Melon
    for (int i = 0; i < melonsAmount; i++)
    {        
        if (melons[i].pos.x <= 0)
        {
            melons[i].pos.x = windowWidth + GetRandomValue(2000,20000);
        }
        else
        {
            melons[i].pos.x -= (ObjectSpeed) * dt;
        }
    }
    
    Vector2 lineStartPosition = 
    {
        0, (windowHeight - 350.0f) + dinoAnimData.rec.height
    };
    Vector2 lineEndPosition = 
    {
        (float)windowWidth, (windowHeight - 350.0f) + dinoAnimData.rec.height
    };
    DrawLineV(lineStartPosition, lineEndPosition, BLACK);
    
    DrawEnvironment();
    
    lineStartPosition.y = (windowHeight - 250) + dinoAnimData.rec.height;
    lineEndPosition.y = (windowHeight - 250) + dinoAnimData.rec.height;
    DrawLineV(lineStartPosition, lineEndPosition, BLACK);

    DrawTextureRec(dinoAnimData.texture, dinoAnimData.rec, dinoAnimData.pos, WHITE);
    DrawLaevas();
    for (int i = 0; i < melonsAmount; i++)
    {
        DrawTextureRec(melonTexture, melons[i].rec, melons[i].pos, WHITE);
    }

    const char* pointsText = TextFormat("Score: %i", pointNum);
    const int scoreTextFontSize = 40;
    Vector2 scoreTextSize = MeasureTextEx(GetFontDefault(), pointsText, scoreTextFontSize, 0);
    Vector2 scoreTextPosition = GetScreenCenter();
    scoreTextPosition.x -= scoreTextSize.x / 2.0f;
    scoreTextPosition.y = 100;
    DrawText(pointsText, scoreTextPosition.x, scoreTextPosition.y, scoreTextFontSize, RED);
}

void DrawDeadScene(void)
{
    const int x = 100;
    DrawText("Game Over, Glinoc Died :(", x, 100, 60, RED);
    DrawText("ENTER TO RESTART", x, 200, 60, RED);
    const char* pointsText = TextFormat("Score: %i", pointNum);
    DrawText(pointsText, x, 300, 30, BLUE);
    DrawControls();
    
    if (IsKeyPressed(KEY_ENTER))
    {
        pointNum = 0;
        ResetLaveas();
        for (int i = 0; i < melonsAmount; i++)
        {
            melons[i].pos.x = windowWidth + GetRandomValue(2000,20000);
        }
        ObjectSpeed = 300;
        ResetEnvironmentSpeed();
        alive = true;
        PlayMusicStream(PixelKingMusic);
    }
}

void EndGameplayScene(void)
{
    UnloadTexture(melonTexture);
    MemFree(melons);
    UnloadLaevas();
    UnloadEnvironment();
    UnloadDino();
    UnloadSound(JumpSound);
    UnloadSound(melonPickUpSound);
    UnloadSound(killSound);
    UnloadMusicStream(PixelKingMusic);
}
