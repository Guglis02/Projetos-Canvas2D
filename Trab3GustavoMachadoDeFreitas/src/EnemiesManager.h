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

    vector<vector<Enemy *>> swarm;

private:
    const int EnemySize = 32;
    const int EnemyPadding = 10;
    const float EnemyCooldown = 2.5;
    float timeSinceLastEnemy = EnemyCooldown;
    bool nextEnemySpawnsLeft = false;

    function<void(int)> enemyDeathCallback;
    function<void(VectorHomo)> enemyShotCallback;

    int swarmColumns;
    int swarmRows;
    int swarmX;
    int swarmY;
    int swarmSpacing;

    void InstantiateEnemy()
    {
        // Procura por uma posição vazia no enxame
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

        // Se não encontrou, não instancia
        if (row == -1 && col == -1)
        {
            return;
        }

        // Calcula posição de spawn do inimigo
        float spawnX = (nextEnemySpawnsLeft) ? -100 : ConstScreenWidth + 100;
        nextEnemySpawnsLeft = !nextEnemySpawnsLeft;
        float spawnY = rand() % ConstScreenHeight;

        // Calcula pontos de controle da trajetória que o inimigo irá percorrer até sua posição no enxame
        VectorHomo position = VectorHomo(spawnX, spawnY);
        VectorHomo target = VectorHomo((col * swarmSpacing) + swarmSpacing + swarmX,
                                       (row * swarmSpacing) + swarmY);

        VectorHomo control1 = VectorHomo((target.x + position.x) / 2, position.y);
        VectorHomo control2 = VectorHomo((target.x + position.x) / 2, target.y);

        // Escolhe aleatoriamente o tipo de inimigo a ser instanciado
        if (rand() % 3 == 1)
        {
            swarm[row][col] = new BomberEnemy(position, enemyDeathCallback, enemyShotCallback);
        }
        else
        {
            swarm[row][col] = new Enemy(position, enemyDeathCallback, enemyShotCallback);
        }
        swarm[row][col]->SetupRoaming(position, target, control1, control2);
    }
};

#endif // ENEMIESMANAGER_H_INCLUDED
