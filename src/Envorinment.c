#include <raylib.h>

typedef struct ObjectData
{
    float movementSpeed;
    Texture2D texture;
    Vector2 position;
} ObjectData;
// From "GameplayScene.c"
extern float ObjectSpeed;

static ObjectData mountainData;
static ObjectData mountainData2;
static ObjectData birdsData;
static ObjectData treesBack;
static ObjectData treesFront;
static ObjectData treesFront2;

static void UpdateMountains(float);
static void UpdateTreesFront(float);

void LoadEnvironment(void)
{
    Texture2D mountainTexture = LoadTexture("textures/mountain.png");
    mountainData.texture = mountainTexture;
    mountainData.position = (Vector2){ 0, 0 };
    mountainData.movementSpeed = 30;

    mountainData2.texture = mountainTexture;
    mountainData2.position = (Vector2){ (float)GetScreenWidth(), 0 };
    mountainData2.movementSpeed = 30;

    birdsData.texture = LoadTexture("textures/birds.png");
    birdsData.position = (Vector2){ 0, 0 };
    birdsData.movementSpeed = 20;

    treesBack.texture = LoadTexture("textures/treesBack.png");
    treesBack.position = (Vector2){ (float)treesBack.texture.width, 0 };
    treesBack.movementSpeed = 70;

    treesFront.texture = LoadTexture("textures/treesFront.png");
    treesFront.position = (Vector2){ (float)treesFront.texture.width, 0 };
    treesFront.movementSpeed = 100; // Unused

    treesFront2.texture = treesFront.texture;
    treesFront2.position = (Vector2){0,0};
    treesFront2.movementSpeed = 100;
}

void UpdateEnvironment(float dt)
{
    // Mountains
    UpdateMountains(dt);

    // Birds
    if (birdsData.position.x <= -birdsData.texture.width)
    {
        birdsData.position.x = birdsData.texture.width;
    }
    else
    {
        birdsData.position.x -= birdsData.movementSpeed * dt;
    }

    // Trees back
    if (treesBack.position.x <= -treesBack.texture.width)
    {
        treesBack.position.x = treesBack.texture.width;
    }
    else
    {
        treesBack.position.x -= treesBack.movementSpeed * dt;
    }

    // Trees front
    UpdateTreesFront(dt);
}

void UpdateMountains(float dt)
{
    if (mountainData.position.x <= -mountainData.texture.width)
    {
        mountainData.position.x = mountainData.texture.width;
    }
    else
    {
        mountainData.position.x -= mountainData.movementSpeed * dt;
    }

    if (mountainData2.position.x <= -mountainData2.texture.width)
    {
        mountainData2.position.x = mountainData2.texture.width;
    }
    else
    {
        mountainData2.position.x -= mountainData2.movementSpeed * dt;
    }
}

void UpdateTreesFront(float dt)
{
    if (treesFront.position.x <= -treesFront.texture.width)
    {
        treesFront.position.x = treesFront.texture.width;
    }
    else
    {
        treesFront.position.x -= ObjectSpeed * dt;
    }

    if (treesFront2.position.x <= -treesFront2.texture.width)
    {
        treesFront2.position.x = treesFront2.texture.width;
    }
    else
    {
        treesFront2.position.x -= ObjectSpeed * dt;
    }
}

void IncreaseEnvironmentSpeed(void)
{
    mountainData.movementSpeed +=2;
    mountainData2.movementSpeed +=2;
    birdsData.movementSpeed += 1;
    treesBack.movementSpeed += 4;
    treesFront.movementSpeed += 6; // Unused
    treesFront2.movementSpeed += 6; // Unused
}

void ResetEnvironmentSpeed()
{
    mountainData.movementSpeed = 30;
    mountainData2.movementSpeed = 30;
    birdsData.movementSpeed = 20;
    treesBack.movementSpeed = 70;
    treesFront.movementSpeed = 100;
    treesFront2.movementSpeed = 100;
}

void DrawEnvironment(void)
{
    DrawTextureV(mountainData.texture, mountainData.position, WHITE);
    DrawTextureV(mountainData2.texture, mountainData2.position, WHITE);
    DrawTextureV(birdsData.texture, birdsData.position, WHITE);
    DrawTextureV(treesBack.texture, treesBack.position, WHITE);
    DrawTextureV(treesFront.texture, treesFront.position, WHITE);
    DrawTextureV(treesFront2.texture, treesFront2.position, WHITE);
}

void UnloadEnvironment(void)
{
    UnloadTexture(mountainData.texture);
    UnloadTexture(birdsData.texture);
    UnloadTexture(treesBack.texture);
    UnloadTexture(treesFront.texture);
}