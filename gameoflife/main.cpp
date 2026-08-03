#include <vector>
#include "raylib.h"

const int CellHeight = 30;
const int CellWidth  = 30;

const int ScreenHeight = 900; 
const int ScreenWidth  = 900;

const int GridCols = ScreenWidth / CellWidth;
const int GridRows = ScreenHeight / CellHeight;

void printCell(int gridX, int gridY)
{
    DrawRectangle(gridX * CellWidth, gridY * CellHeight, CellWidth, CellHeight, WHITE);
}

void drawGrid2d()
{
    for (int x = 0; x <= ScreenWidth; x += CellWidth)
    {
        DrawLine(x, 0, x, ScreenHeight, LIGHTGRAY);
    }
    for (int y = 0; y <= ScreenHeight; y += CellHeight)
    {
        DrawLine(0, y, ScreenWidth, y, LIGHTGRAY);
    }
}

bool DetectMouseClick(int* gridX, int* gridY)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        *gridX = (int)mousePos.x / CellWidth;
        *gridY = (int)mousePos.y / CellHeight;

        if (*gridX >= 0 && *gridX < GridCols && *gridY >= 0 && *gridY < GridRows)
        {
            return true;
        }
    }
    return false;
}

int neighbourCounter(const std::vector<std::vector<bool>>& grid, int posX, int posY)
{
    int aliveNeighbour = 0;
    for (int i = -1; i <= 1; i++) 
    {
        for (int j = -1; j <= 1; j++) 
        {
            if (i == 0 && j == 0) continue;

            int checkX = posX + i;
            int checkY = posY + j;

            if (checkX >= 0 && checkX < GridCols && checkY >= 0 && checkY < GridRows) 
            {
                if (grid[checkX][checkY]) 
                {
                    aliveNeighbour++;
                }
            }
        }
    }
    return aliveNeighbour;
}

bool rules(const std::vector<std::vector<bool>>& grid, int posX, int posY)
{
    int neighbours = neighbourCounter(grid, posX, posY);
    bool isAlive = grid[posX][posY];

    if (isAlive) 
    {
        if (neighbours < 2 || neighbours > 3) return false;
        return true;
    } 
    else 
    {
        if (neighbours == 3) return true;
        return false;
    }
}

int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Game of Life - Spacebar to Play/Pause");
    SetTargetFPS(60);

    int clickedX = 0;
    int clickedY = 0;
    bool isPaused = true;
    float updateTimer = 0.0f;
    float updateDelay = 0.1f;

    std::vector<std::vector<bool>> grid(GridCols, std::vector<bool>(GridRows, false));
    std::vector<std::vector<bool>> nextGrid(GridCols, std::vector<bool>(GridRows, false));

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE)) 
        {
            isPaused = !isPaused;
        }

        if (DetectMouseClick(&clickedX, &clickedY))
        {
            grid[clickedX][clickedY] = true; 
        }

        if (!isPaused)
        {
            updateTimer += GetFrameTime();
            if (updateTimer >= updateDelay)
            {
                updateTimer = 0.0f;
                for (int x = 0; x < GridCols; x++)
                {
                    for (int y = 0; y < GridRows; y++)
                    {
                        nextGrid[x][y] = rules(grid, x, y);
                    }
                }
                grid = nextGrid; // Push temporary layout to operational grid
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (int x = 0; x < GridCols; x++)
        {
            for (int y = 0; y < GridRows; y++)
            {
                if (grid[x][y])
                {
                    printCell(x, y);
                }
            }
        }

        drawGrid2d();

        if (isPaused) DrawText("PAUSED - Click to draw / Press Space to Run", 10, 10, 20, GREEN);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
