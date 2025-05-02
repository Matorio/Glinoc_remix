/*
Original game by: Floky
https://flokydev.itch.io/glino
https://www.youtube.com/watch?v=y27SHJh706A

Modified by Matorio
*/

#include <stdbool.h>
#include <raylib.h>
#include <raymath.h> // Lerp
#include "AnimData.h"
#include "GameplayScene.h"
#include "../textures/Dino.h"

extern const int windowWidth = 1280;
extern const int windowHeight = 720;

bool musicOn = true;
AnimData dinoAnimData = {0};

static void DrawMenuScene(void);
static void DrawRenderTextureInsideScreen(RenderTexture2D);

void DrawControls(void)
{
    DrawText("CONTROLS", 850, 450, 40, RED);
    DrawText(">>SPACE<< TO JUMP OVER AXES", 800, 500, 20, BLUE);
    DrawText(">>S<< TO DUCK UNDER AXES", 800, 550, 20, BLUE);
    DrawText(">>M<< TO TOGGLE MUSIC",800, 600, 20, BLUE);
}

int main()
{
    InitWindow(1280, 720, "Glinoc");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowMinSize(800, 450);
    RenderTexture2D renderTexture = LoadRenderTexture(windowWidth, windowHeight);
    //Sounds
    InitAudioDevice();

    //Textures Dino
    Image dinoImage = LoadImageFromMemory(".png", Dino_data, Dino_size);
    Texture2D Dino = LoadTextureFromImage(dinoImage);
    UnloadImage(dinoImage);
    dinoAnimData.texture = Dino;
    dinoAnimData.rec.width = Dino.width/4;
    dinoAnimData.rec.height = Dino.height;
    dinoAnimData.rec.x = 0;
    dinoAnimData.rec.y = 0;
    dinoAnimData.pos.x = windowWidth/3 - dinoAnimData.rec.width/2;
    dinoAnimData.pos.y = ((windowHeight - 80) - dinoAnimData.rec.height);
    dinoAnimData.frame = 0;
    dinoAnimData.runntingTime = 0;
    dinoAnimData.updateTime =  0.1;

    float musicTextAlpha = 0;

    // Menu scene
    bool Menu = true;
    const Color BG_color = {240, 245, 245, 255};
  
    SetTargetFPS(40);
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_M))
        {
            musicOn = !musicOn;
            musicTextAlpha = 1;
        }

        BeginDrawing();
        ClearBackground(BLANK);
        BeginTextureMode(renderTexture);
        ClearBackground(BG_color);

        if(Menu)
        {
            DrawMenuScene();
            
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP))
            {
                Menu = false;
                StartGameplayScene();
            }
            if (IsKeyPressed(KEY_Q))
            {
                musicOn = false;
            }            
        }
        else
        {
            UpdateGameplayScene();
            DrawGameplayScene();
        }
       
        Color musicTextColor = BLACK;
        musicTextColor.a = (unsigned char)(musicTextAlpha * 255);
        DrawText(musicOn ? "Music: ON" : "Music: OFF", 20, 20, 25, musicTextColor);

        musicTextAlpha = Lerp(musicTextAlpha, 0.0f, GetFrameTime() * 5);
        EndTextureMode();
        DrawRenderTextureInsideScreen(renderTexture);
        EndDrawing();
    }
    // If we were playing
    if(Menu == false)
    {
        EndGameplayScene();
    }
    UnloadTexture(Dino);
    UnloadRenderTexture(renderTexture);
    CloseWindow();
    return 0;
}

void DrawMenuScene(void)
{
    DrawText("Welcome to the Story of Glinoc!", 100, 100, 40, BLACK);
    DrawText("It wasn't asteriods that killed the Dinos...", 100, 150, 40, BLACK);
    DrawText("It was an axe called Laeva!", 100, 200, 40, RED);
    DrawText("Help Glinoc avoiding avoiding certain DEATH!", 100, 250, 40, BLACK);
    DrawTextureRec(dinoAnimData.texture, dinoAnimData.rec, dinoAnimData.pos, WHITE);
    DrawText("ENTER TO START", 100, 400, 60, RED);
    DrawControls();
}

void DrawRenderTextureInsideScreen(RenderTexture2D renderTexture)
{
    const int width = GetScreenWidth();
    const int height = GetScreenHeight();
    Rectangle source = 
    {
        0, 0,
        windowWidth, windowHeight
    };
    Rectangle target = source;
    float scaleX = (float)width / (float)windowWidth;
    float scaleY = (float)height / (float)windowHeight;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;
    target.width *= scale;
    target.height *= scale;
    target.x = width / 2.0f;
    target.y = height / 2.0f;
    const Vector2 origin = {target.width / 2.0f, target.height / 2.0f};
    source.height *= -1;
    DrawTexturePro(renderTexture.texture, source, target, origin, 0.0f, WHITE);
}