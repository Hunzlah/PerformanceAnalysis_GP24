#include "Game.h"
#include "resource_dir.h"
#include "Textures.h"
#include "MainMenu.h"
#include "GameStates.h"
#include "GameOver.h"
int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Defend the Spire");
    SearchAndSetResourceDir("resources");
    LoadTextures();
    InitializeLevelSettings();
    InitializeGameSettings();
    SetTargetFPS(60);
    bool isManualExit = false;
    while (!WindowShouldClose())
    {
        switch (currentGameState)
        {
        case MainMenu:
        if (currentMenuScreen == EXIT)
            {
                isManualExit = true;
            }
            else
            {
                MainMenuHandler();
            }
            break;
        case Gameplay:
            GameHandler();
            break;
        case GameOver:
            GameOverHandler();
            break;

        default:
            break;
        }
        if(isManualExit) break;
    }

    UnloadTextures();
    CloseWindow();
    return 0;
}
