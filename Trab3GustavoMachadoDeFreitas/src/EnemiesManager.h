#ifndef ENEMIESMANAGER_H_INCLUDED
#define ENEMIESMANAGER_H_INCLUDED

#include "./Entities/Enemy.h"
#include "./Utils/VectorHomo.h"
#include "./Utils/FpsController.h"

#include <vector>

using namespace std;

class EnemiesManager
{
public:
    EnemiesManager(int screenWidth, int screenHeight, int borderWidth)
    {
        float swarmWidth = screenWidth - (borderWidth << 1);
        float swarmHeight = screenHeight >> 1;

        this->swarmX = borderWidth;
        this->swarmY = swarmHeight;

        this->swarmSpacing = 2 * EnemySize + EnemyPadding;
        this->swarmColumns = swarmWidth / swarmSpacing;
        this->swarmRows = swarmHeight / swarmSpacing - 1;

        // Initialize the 2D vector with nullptrs
        swarm.resize(swarmRows, vector<Enemy*>(swarmColumns, nullptr));
    }

    void Update()
    {
        timeSinceLastEnemy += FpsController::getInstance().GetDeltaTime();

        if (timeSinceLastEnemy >= EnemyCooldown)
        {
            InstantiateEnemy();
            timeSinceLastEnemy = 0;
        }

        for (auto& row : swarm)
        {
            for (auto& enemy : row)
            {
                if (enemy != nullptr)
                {
                    enemy->Update();
                }
            }
        }
    }

    vector<vector<Enemy*>> swarm;
private:
    const int EnemySize = 32;
    const int EnemyPadding = 10;
    const float EnemyCooldown = 0.5;
    float timeSinceLastEnemy = EnemyCooldown;

    int swarmColumns;
    int swarmRows;
    int swarmX;
    int swarmY;
    int swarmSpacing;

    void InstantiateEnemy()
    {
        int row = -1, col = -1;
        for (int i = 0; i < swarmRows; i++)
        {
            for (int j = 0; j < swarmColumns; j++)
            {
                if (swarm[i][j] == nullptr)
                {
                    row = i;
                    col = j;
                    break;
                }
            }
        }

        if (row == -1 && col == -1)
        {
            return;
        }

        VectorHomo position = VectorHomo((col * swarmSpacing) + swarmSpacing + swarmX,
                                         (row * swarmSpacing) + swarmY);
        swarm[row][col] = new Enemy(position);
    }
};

#endif // ENEMIESMANAGER_H_INCLUDED
