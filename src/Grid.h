#pragma once
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Constants.h"
#include "HelperMethods.h"
#include <iostream>
#include "stack"
#include "queue"
#include <bits/stdc++.h>
#include "Textures.h"
using namespace std;


enum CellType {
    EMPTY,
    WALL,
    TOWER
};
struct Cell
{
    CellType cellType;
    int distance;
    bool isVisited;
    int positionX;
    int positionY;
};

// Grid class handling the grid logic
class Grid {
    public:
        Grid() {
            grid.resize(rows, vector<Cell>(cols));
            
        }
        void Initialize(){
            for (int y = 0; y < rows; y++) 
            {
                for (int x = 0; x < cols; x++) 
                {
                    Cell cell = {
                        EMPTY,
                        0,
                        false,
                        x,
                        y
                    };
                    grid[y][x] = cell;
                }
            }

            AddWalls();
        }
        bool IsPointOutOfBounds(int x, int y)
        {
            bool found = false;

            if(x < 0 || y < 0 || x >= cols || y >= rows) found = true;

            return found;

        }
        bool IsVisited(int x, int y){
            return grid[y][x].isVisited;
        }
        void SetPathToTower(int x, int y)
        {
            queue<Cell> frontier;
            Cell origin = GetCell(x,y);
            SetCellDistance(origin.positionX, origin.positionY, 0);
            SetCellIsVisited(origin.positionX, origin.positionY);
            frontier.push(origin);
            while (frontier.size() != 0)
            {
                Cell current = frontier.front();
                frontier.pop();
                vector<Cell> neighbors = GetNeighbors(current.positionX, current.positionY);
                for(const Cell& neighbor : neighbors)
                {
                    if(!neighbor.isVisited)
                    {
                        frontier.push(neighbor);
                        int posX = neighbor.positionX;
                        int posY = neighbor.positionY;
                        SetCellIsVisited(posX, posY);
                        SetCellDistance(posX, posY, GetCellDistance(current.positionX, current.positionY) + 1);
                    }
                }
            }
        }
        vector<Cell> GetNeighbors(int x, int y)
        {
            vector<Cell> neighbors;
            if(!IsPointOutOfBounds(x, y+1) && !IsWall(x,y+1)) neighbors.push_back(GetCell(x,y+1));
            if(!IsPointOutOfBounds(x, y-1) && !IsWall(x,y-1)) neighbors.push_back(GetCell(x,y-1));
            if(!IsPointOutOfBounds(x+1, y) && !IsWall(x+1,y)) neighbors.push_back(GetCell(x+1,y));
            if(!IsPointOutOfBounds(x-1, y) && !IsWall(x-1,y)) neighbors.push_back(GetCell(x-1,y));
            return neighbors;
        }
        int GetCellDistance(int x, int y)
        {
            return grid[y][x].distance;
        }
        void SetCellIsVisited(int x, int y)
        {
            grid[y][x].isVisited = true;
        }
        void SetCellDistance(int x, int y, int distance)
        {
            grid[y][x].distance = distance;
        }
        void AddWalls() 
        {
            for (int y = 0; y < rows; y++) 
            {
                for (int x = 0; x < cols; x++) 
                {
                    if(GetRandomNumberInRange(10) > 7)
                    {
                        grid[y][x].cellType = WALL;
                    }
                }
            }
        }
    
        void Draw() {
            for (int y = 0; y < rows; y++) {
                for (int x = 0; x < cols; x++) {
                    //Color color = LIGHTGRAY;
                    //if (grid[y][x].cellType == WALL) color = DARKGRAY;
                    //else if (grid[y][x].cellType == TOWER) color = RED;
    
                    //DrawRectangleLines(x * cellSize, y * cellSize, cellSize, cellSize, color);
                    if (grid[y][x].cellType == WALL) {
                        DrawTexture(wallTexture, x * cellSize, y * cellSize, WHITE);
                        //DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, color);
                    } 
                    else if (grid[y][x].cellType == TOWER) {
                        DrawTexture(castleTexture, x * cellSize, y * cellSize, WHITE);
                        //DrawCircle(x * cellSize + cellSize / 2, y * cellSize + cellSize / 2, 10, RED);
                    }else if(grid[y][x].cellType == EMPTY)
                    {
                        DrawTexture(groundTexture, x * cellSize, y * cellSize, WHITE);
                    }
                    //DrawText(IntToCharPointer(grid[y][x].distance), 
                    //x * cellSize + cellSize / 2, y * cellSize + cellSize / 2, 10, BLACK);
                }
            }
        }
    
        bool IsWall(int x, int y) const {
            return grid[y][x].cellType == WALL;
        }
    
        void SetTowerCell(int x, int y, CellType type) {
            grid[y][x].cellType = type;
        }
        Cell GetCell(int x, int y)
        {
            return grid[y][x];
        }
        Cell GetNextPoint(int x, int y){
            int currentDistance = GetCell(x,y).distance;
            Cell next = GetCell(x,y);
            vector<Cell> neighbors = GetNeighbors(x,y);
            ShuffleNeighbours(neighbors);
            for(const Cell& neighbor : neighbors)
            {
                if(currentDistance > neighbor.distance)
                {
                    next = neighbor;
                    break;
                }
            }
            return next;
        }
        vector<Cell> ShuffleNeighbours(vector<Cell> neighbours)
        {
            random_shuffle(neighbours.begin(), neighbours.end());
            return neighbours;
        }
    
    private:
        vector<vector<Cell>> grid;
    };