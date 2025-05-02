#include <raylib.h>
#include "AnimData.h"
#include <stdbool.h>

//Dino Is ducking
static bool isDuck = false;
// Dino vertical velocity 
static int Dino_velocity = 0;
// Dino Jump Hight (pixels/s/s)/frame
const int jump_hight = 800;
//Gravity (pixels/s/s)/frame
const int gravity = 2000;
// From "main.c"
extern AnimData dinoAnimData;
// From "main.c"
extern const int windowHeight;
//Duck cooldown
static float DuckRunningTime = 0;
static float DuckUpdateTime = 0.45f;

static Sound jumpSound;

static bool IsOnGround(AnimData data)
{
   return data.pos.y >= (windowHeight - 80) - data.rec.height;
}

void LoadDino(void)
{
    jumpSound = LoadSound("Sounds/jump.mp3");
}

void UpdateDino(void)
{
    float dt = GetFrameTime();
    bool isGrounded = IsOnGround(dinoAnimData);
    // Duck
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
    {
        isDuck = true;      
    }

    // Ground check
    if (isGrounded)
    {
        Dino_velocity = 0;
    }
    else
    {
        // Apply gravity
        Dino_velocity += gravity * dt * ((isDuck*2)+1);
    }

    if (isDuck && DuckRunningTime < DuckUpdateTime)
    {
        DuckRunningTime += dt;

        dinoAnimData.rec.x = 3*dinoAnimData.rec.width;
    }
    else
    {
        DuckRunningTime = 0.0;
        isDuck = false;

        //update animData Dino
        dinoAnimData = UpdateAnimData(dinoAnimData, dt, 2);
    }

    // Jump
    bool jumpKey = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
    if (isGrounded && jumpKey)
    {
        Dino_velocity -= jump_hight;
        PlaySound(jumpSound);
    }

    dinoAnimData.pos.y += Dino_velocity * dt;

    const float limit = (windowHeight - 80) - dinoAnimData.rec.height;

    if (dinoAnimData.pos.y > limit)
    {
        dinoAnimData.pos.y = limit;
    }
}

bool DinoIsDuck(void)
{
    return isDuck;
}

void UnloadDino(void)
{
    UnloadSound(jumpSound);
}