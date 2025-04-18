#pragma once
#include <cstdio>  // for snprintf
#include <cstdlib> // for malloc / free
#include "raylib.h"
#include "GameStates.h"
struct Button {
    Rectangle rect;
    const char* text;
};

char* IntToCharPointer(int value) {
    // Allocate enough space for the string (max 11 chars for int32 + null terminator)
    char* result = new char[12]; 
    std::snprintf(result, 12, "%d", value); // Convert int to string
    return result;
}
float GetRandomNumberInRange(int max)
{
    return (float)(rand() % max);
}

// Function to check if a button is clicked
bool IsButtonClicked(Button button) {
    return (CheckCollisionPointRec(GetMousePosition(), button.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

void InitializeLevelSettings()
{
    castleHp = 50;
    levelPassTime = 20 * currentLevel;
    levelTimer = 0;
}
