#include "Game.h"
#include "resource_dir.h"
#include "Textures.h"
#include "MainMenu.h"
#include "GameStates.h"
#include "GameOver.h"
#include "Profiler.h"
int main() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Defend the Spire");
    SearchAndSetResourceDir("resources");
    //Profiler::Get().BeginFrame();
    {
        ProfilerScopeConstant scope("Load Textures");
        LoadTextures();
    }
    //Profiler::Get().EndFrame();
    currentGameState = Gameplay;
    InitializeLevelSettings();
    {
        ProfilerScopeConstant scope("BFS");
        InitializeGameSettings();
    }
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

    {
        ProfilerScopeConstant scope("Unload Textures");
        UnloadTextures();
    }

    Profiler::Get().EndSession("ProfilerData");

    CloseWindow();
    return 0;
}
