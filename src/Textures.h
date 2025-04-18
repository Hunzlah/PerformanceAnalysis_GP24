#pragma once
#include "raylib.h"

Texture2D groundTexture, wallTexture, enemyTexture, bombTexture, castleTexture;

void LoadTextures()
{
    wallTexture = LoadTexture("wall.png");
    groundTexture = LoadTexture("ground.png");
    enemyTexture = LoadTexture("enemy.png");
    castleTexture = LoadTexture("castle.png");
}
void UnloadTextures()
{
    UnloadTexture(wallTexture);
    UnloadTexture(groundTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(castleTexture);
}