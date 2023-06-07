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
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

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
    int screenWidth;
    int screenHeight;

    const int EnemySize = 32;
    const int EnemyPadding = 10;
    const float EnemyCooldown = 60;
    float timeSinceLastEnemy = EnemyCooldown;
    bool nextEnemySpawnsLeft = false;    

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

        float spawnX = (nextEnemySpawnsLeft) ? -100 : screenWidth + 100;
        nextEnemySpawnsLeft = !nextEnemySpawnsLeft;
        float spawnY = rand() % screenHeight;

        VectorHomo position = VectorHomo(spawnX, spawnY);
        VectorHomo target = VectorHomo((col * swarmSpacing) + swarmSpacing + swarmX,
                                         (row * swarmSpacing) + swarmY);        
                                         
        VectorHomo control1 = VectorHomo((target.x + position.x) / 2, position.y);
        VectorHomo control2 = VectorHomo((target.x + position.x) / 2, target.y);

        swarm[row][col] = new Enemy(position);
        swarm[row][col]->SetupRoaming(position, target, control1, control2);
    }
};

#endif // ENEMIESMANAGER_H_INCLUDED