#ifndef ENEMIESMANAGER_H_INCLUDED
#define ENEMIESMANAGER_H_INCLUDED

#include "./Entities/Enemy.h"
#include "./Entities/BomberEnemy.h"
#include "./Utils/VectorHomo.h"
#include "./Utils/FpsController.h"
#include "./Utils/GlobalConsts.h"

#include <vector>

using namespace std;

// Classe responsável por gerenciar os inimigos do jogo
class EnemiesManager
{
public:
    EnemiesManager(int borderWidth)
    {
        // Calcula dimensões do enxame
        float swarmWidth = ConstScreenWidth - (borderWidth << 1);
        float swarmHeight = ConstScreenHeight >> 1;

        // Calcula posição inicial do enxame
        this->swarmX = borderWidth;
        this->swarmY = swarmHeight + EnemySize + EnemyPadding;

        // Calcula disposição dos inimigos
        this->swarmSpacing = 2 * (EnemySize + EnemyPadding);
        this->swarmColumns = swarmWidth / swarmSpacing - 1;
        this->swarmRows = swarmHeight / swarmSpacing - 1;

        swarm.resize(swarmRows, vector<Enemy *>(swarmColumns, nullptr));
    }

    void SetCallbacks(function<void(int)> enemyDeathCallback,
                      function<void(VectorHomo)> enemyShotCallback)
    {
        this->enemyDeathCallback = enemyDeathCallback;
        this->enemyShotCallback = enemyShotCallback;
    }

    void Update()
    {
        timeSinceLastEnemy += FpsController::getInstance().GetDeltaTime();
        timeSinceLastCdReduction += FpsController::getInstance().GetDeltaTime();

        if (timeSinceLastCdReduction >= reduceEnemySpawnCdEvery)
        {
            EnemyCooldown = max(EnemyCooldown - enemySpawnCdReduction, 0.4f);

            timeSinceLastCdReduction = 0;
        }

        if (timeSinceLastEnemy >= EnemyCooldown)
        {
            InstantiateEnemy();
            timeSinceLastEnemy = 0;
        }

        for (auto &row : swarm)
        {
            for (auto &enemy : row)
            {
                if (enemy != nullptr)
                {
                    enemy->Update();
                }
            }
        }
    }

    void Reset()
    {
        for (auto &row : swarm)
        {
            for (auto &enemy : row)
            {
                if (enemy != nullptr)
                {
                    delete enemy;
                    enemy = nullptr;
                }
            }
        }
    }

    bool CheckCollision(vector<VectorHomo> hitbox)
    {
        for (int i = 0; i < swarmRows; i++)
        {
            for (int j = 0; j < swarmColumns; j++)
            {
                if (swarm[i][j] != nullptr && swarm[i][j]->CheckCollision(hitbox))
                {
                    swarm[i][j]->OnHit();
                    swarm[i][j] = nullptr;
                    return true;
                }
            }
        }

        return false;
    }

    Enemy* GetRandomEnemy()
    {
        for (int i = 0; i < swarmRows; i++)
        {
            for (int j = 0; j < swarmColumns; j++)
            {
                if (swarm[i][j] != nullptr)
                {
                    return swarm[i][j];
                }
            }
        }

        return nullptr;
    }

    vector<vector<Enemy *>> swarm;

private:
    const int EnemySize = 32;
    const int EnemyPadding = 10;

    float EnemyCooldown = 1.5;
    float timeSinceLastEnemy = EnemyCooldown;
    bool nextEnemySpawnsLeft = false;

    // A cada 10 segundos reduz o cooldown dos inimigos em 0.1s
    float reduceEnemySpawnCdEvery = 10;
    float enemySpawnCdReduction = 0.1;
    float timeSinceLastCdReduction = 0;

    function<void(int)> enemyDeathCallback;
    function<void(VectorHomo)> enemyShotCallback;

    int swarmColumns;
    int swarmRows;
    int swarmX;
    int swarmY;
    int swarmSpacing;

    // Procura por uma posição vazia no enxame
    // Começa buscando pelo meio do enxame e vai aumentando o raio de busca  
    VectorHomo FindEmptySpot()
    {
        int row = -1, col = -1;
        int middleRow = swarmRows / 2;
        int middleCol = swarmColumns / 2;
        int radius = 1;
        bool foundEmptyPosition = false;

        while (!foundEmptyPosition)
        {
            for (int i = middleRow - radius; i <= middleRow + radius; i++)
            {
                for (int j = middleCol - radius; j <= middleCol + radius; j++)
                {
                    if (i >= 0 && i < swarmRows && j >= 0 && j < swarmColumns && swarm[i][j] == nullptr)
                    {
                        row = i;
                        col = j;
                        foundEmptyPosition = true;
                        break;
                    }
                }
                if (foundEmptyPosition)
                {
                    break;
                }
            }
            radius++;
            if (radius > swarmRows && radius > swarmColumns)
            {
                break;
            }
        }

        return VectorHomo(col, row);
    }

    void InstantiateEnemy()
    {
        // Procura por uma posição vazia no enxame
        VectorHomo emptySpot = FindEmptySpot();

        // Se não encontrou, não instancia
        if (emptySpot.x == -1 && emptySpot.y == -1)
        {
            return;
        }

        // Calcula posição de spawn do inimigo
        float spawnX = (nextEnemySpawnsLeft) ? -100 : ConstScreenWidth + 100;
        nextEnemySpawnsLeft = !nextEnemySpawnsLeft;
        float spawnY = rand() % ConstScreenHeight;

        VectorHomo spawnPosition = VectorHomo(spawnX, spawnY);
        VectorHomo targetPosition = VectorHomo((emptySpot.x * swarmSpacing) + swarmSpacing + swarmX,
                                       (emptySpot.y * swarmSpacing) + swarmY);

        // Escolhe aleatoriamente o tipo de inimigo a ser instanciado
        if (rand() % 3 == 1)
        {
            swarm[emptySpot.y][emptySpot.x] = new BomberEnemy(spawnPosition, targetPosition, enemyDeathCallback, enemyShotCallback);
        }
        else
        {
            swarm[emptySpot.y][emptySpot.x] = new Enemy(spawnPosition, targetPosition, enemyDeathCallback, enemyShotCallback);
        }
    }
};

#endif // ENEMIESMANAGER_H_INCLUDED
