#pragma once
enum GameState
{
    MainMenu,
    Gameplay,
    GameOver
};
enum MenuScreen { MENU, PLAY, HOW_TO_PLAY, CREDITS, EXIT };
enum GameOverState {
    LEVEL_PASSED,
    LEVEL_FAILED
};
GameState currentGameState = MainMenu;
MenuScreen currentMenuScreen = MENU;
GameOverState gameOverState = LEVEL_PASSED;

int currentScore;
int currentLevel = 1;

int castleHp;
float levelPassTime;
float levelTimer;
