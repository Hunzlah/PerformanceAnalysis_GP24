#pragma once
#include "raylib.h"
#include "Grid.h"
#include "Constants.h"
#include "Enemy.h"
#include "GameStates.h"

Grid grid;
vector<Enemy> enemies;
float enemySpawnTimer;
float enemySpawnDelay;
int enemyCounter;

void InitializeGameSettings()
{
    // Tower at center
    int towerX = cols / 2;
    int towerY = rows / 2;
    grid.Initialize();
    grid.SetTowerCell(towerX, towerY, TOWER);
    grid.SetPathToTower(towerX, towerY);

    enemySpawnDelay = 0.5f;
}
void RemoveEnemy(Enemy* enemy)
{
    int enemyId = enemy->Id;
    enemies.erase(std::remove_if(enemies.begin(), // from start of list
                                  enemies.end(),   // to the end
                                  [enemyId](const Enemy &e)
                                  {
                                      return e.Id == enemyId;
                                  }), //  moves all items to be removed to the end of the 
                                      // list, then returns start of range to erase
                   enemies.end());
}
void SetEnemyNextTargetPosition(Enemy* enemy)
{
    Cell enemyTargetCell = grid.GetNextPoint(enemy->cellPostion.x, enemy->cellPostion.y);

    //Enemy reached to final target - Destroy it
    if(enemy->cellPostion.x == enemyTargetCell.positionX && enemy->cellPostion.y == enemyTargetCell.positionY)
    {
        RemoveEnemy(enemy);
        castleHp = castleHp - 1 <= 0 ? 0 : castleHp-1;
        if(castleHp == 0)
        {
            gameOverState = LEVEL_FAILED;
            currentGameState = GameOver;
        }
        return;
    }

    enemy->SetCellPosition((Vector2){enemyTargetCell.positionX, enemyTargetCell.positionY});
    Vector2 targetPos = {enemyTargetCell.positionX * cellSize + cellSize / 2, enemyTargetCell.positionY*cellSize + cellSize / 2};
    enemy->SetTarget(targetPos);
}
Vector2 GetRandomSpawnCellPosition()
{
    Vector2 spawnPos;
    int randNum = GetRandomNumberInRange(4);
    switch (randNum)
    {
    case 0:
        spawnPos = {GetRandomNumberInRange(cols), 0};
        break;
    case 1:
        spawnPos = {0, GetRandomNumberInRange(rows)};
        break;
    case 2:
        spawnPos = {GetRandomNumberInRange(cols), rows - 1};
        break;
    case 3:
        spawnPos = {cols - 1, GetRandomNumberInRange(rows)};
        break;

    default:
        spawnPos = {GetRandomNumberInRange(cols), 0};
        break;
    }

    return spawnPos;
}
void SpawnNewEnemy()
{
    // Enemy starts at top-left corner
    Vector2 enemyCellPos = GetRandomSpawnCellPosition();
    while (grid.GetCell(enemyCellPos.x, enemyCellPos.y).cellType == WALL)
    {
        enemyCellPos = GetRandomSpawnCellPosition();
    }
    
    Cell enemyCell = grid.GetCell(enemyCellPos.x, enemyCellPos.y);
    Vector2 enemyPos = {enemyCell.positionX * cellSize + cellSize/2, enemyCell.positionY * cellSize + cellSize/2};
    Enemy enemy(enemyPos, enemyCounter++);
    enemy.SetCellPosition(enemyCellPos);

    
    SetEnemyNextTargetPosition(&enemy);

    enemies.push_back(enemy);
}
Vector2 GetClickedCell() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        int cellX = (int)(mousePos.x) / cellSize;
        int cellY = (int)(mousePos.y) / cellSize;
        cout<<cellX<<", "<<cellY<<endl;
        return { (float)cellX, (float)cellY };
    }

    return { -1, -1 }; // Return -1, -1 if no click detected
}
void CheckPlayerCollisionWithEnemies()
{
    Vector2 clickedCellPos = GetClickedCell();
    if(clickedCellPos.x == -1 || clickedCellPos.y == -1) return;

    Cell clickedCell = grid.GetCell(clickedCellPos.x, clickedCellPos.y);

    vector<Enemy> enemiesHit;
    for(Enemy& enemy : enemies)
    {
        Rectangle cellRect = {
            clickedCell.positionX * cellSize + cellSize/2,
            clickedCell.positionY * cellSize + cellSize/2,
            (float)cellSize,
            (float)cellSize
        };
        if(CheckCollisionCircleRec(enemy.position, enemy.size, cellRect))
        {
            enemiesHit.push_back(enemy);
        }
    }

    for(Enemy& enemy : enemiesHit)
    {
        RemoveEnemy(&enemy);
    }
}
void DrawGameStats()
{
    char result[100];
    strcpy(result, "Castle HP: ");
    strcat(result, std::to_string(castleHp).c_str());
    DrawText(result, screenWidth / 2 - 100, screenHeight - 50, 30,
             currentScore < 100 ? RED : GREEN);

    strcpy(result, "Level ");
    strcat(result, std::to_string(currentLevel).c_str());
    DrawText(result, screenWidth / 2 + 250, screenHeight - 50, 15, DARKGREEN);

    strcpy(result, "Time: ");
    int levelTimerInt = levelPassTime - levelTimer;
    strcat(result, std::to_string(levelTimerInt).c_str());
    DrawText(result, screenWidth / 2 - 350, screenHeight - 50, 30,
        WHITE);
}
void GameHandler()
{
    BeginDrawing();
        ClearBackground(BLACK);
        CheckPlayerCollisionWithEnemies();
        enemySpawnTimer += GetFrameTime();
        levelTimer += GetFrameTime();
        if(levelTimer >= levelPassTime)
        {
            gameOverState = LEVEL_PASSED;
            currentGameState = GameOver;
            return;
        }
        if(enemySpawnTimer > 0.5f)
        {
            SpawnNewEnemy();
            enemySpawnTimer = 0;
            enemySpawnDelay = GetRandomNumberInRange(7)/10;
        }
        grid.Draw();
        for(Enemy& enemy : enemies )
        {
            enemy.Update();
            if(enemy.CurrentDistanceToTarget() <= 1)
            {
                SetEnemyNextTargetPosition(&enemy);
            }
            enemy.Draw();
        }
        DrawGameStats();
        
        EndDrawing();
}