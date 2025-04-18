#pragma once
#include "Constants.h"
#include "GameStates.h"
#include "HelperMethods.h"

// Buttons
Button nextLevelBtn = { { screenWidth / 2 - 75, 250, 150, 50 }, "Next Level" };
Button retryBtn = { { screenWidth / 2 - 75, 250, 150, 50 }, "Retry"};
Button mainMenuBtn = { { screenWidth / 2 - 75, 320, 150, 50 }, "Main Menu"};


void GameOverHandler(){
    
    BeginDrawing();
        ClearBackground(RAYWHITE);

        if (gameOverState == LEVEL_PASSED) {
            DrawText("Level Passed!", screenWidth / 2 - 100, 100, 30, GREEN);
            
            // Draw Next Level Button
            DrawRectangleRec(nextLevelBtn.rect, GRAY);
            DrawText(nextLevelBtn.text, nextLevelBtn.rect.x + 20, nextLevelBtn.rect.y + 15, 20, WHITE);
            
            if (IsButtonClicked(nextLevelBtn)) {
                currentLevel += 1;
                InitializeGameSettings();
                InitializeLevelSettings();
                currentGameState = Gameplay;
            }
        } 
        else if (gameOverState == LEVEL_FAILED) {
            DrawText("Level Failed!", screenWidth / 2 - 100, 100, 30, RED);
            
            // Draw Retry Button
            DrawRectangleRec(retryBtn.rect, GRAY);
            DrawText(retryBtn.text, retryBtn.rect.x + 50, retryBtn.rect.y + 15, 20, WHITE);
            
            if (IsButtonClicked(retryBtn)) {
                InitializeGameSettings();
                InitializeLevelSettings();
                currentGameState = Gameplay;
            }
        }

        // Draw Main Menu Button (common for both states)
        DrawRectangleRec(mainMenuBtn.rect, GRAY);
        DrawText(mainMenuBtn.text, mainMenuBtn.rect.x + 20, mainMenuBtn.rect.y + 15, 20, WHITE);

        if (IsButtonClicked(mainMenuBtn)) {
            currentGameState = MainMenu;
        }

        EndDrawing();


}