#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "./Utils/KeyboardHandler.h"
#include "./Utils/VectorHomo.h"
#include "BorderController.h"
#include "Entities/Player.h"
#include "Entities/EnemyProjectile.h"
#include "Entities/FriendlyProjectile.h"
#include "Entities/Enemy.h"
#include "./Utils/FpsController.h"
#include "./Utils/GlobalConsts.h"
#include "UIManager.h"
#include "EnemiesManager.h"

class GameManager
{
public:
    GameManager()
    {
        this->keyboardHandler = new KeyboardHandler();

        this->leftBorder = new BorderController(BorderWidth);
        this->rightBorder = new BorderController(ConstScreenWidth - BorderWidth);

        this->enemiesManager = new EnemiesManager(BorderWidth);
        this->enemiesManager->SetCallbacks(bind(&GameManager::EnemyDeathCallback, this, placeholders::_1),
                                           bind(&GameManager::InstantiateEnemyProjectile, this, placeholders::_1));

        this->player = new Player(VectorHomo(ConstScreenWidth >> 1, 200), bind(&GameManager::InstantiatePlayerProjectile, this));
        this->uiManager = new UIManager();

        SetKeyboardCallbacks();
    }

    void Update()
    {
        FpsController::getInstance().updateFrames();

        leftBorder->Update(100.0);
        rightBorder->Update(100.0);

        if (flag == 1)
        {
            PaintBackground();
        }
        else
        {
            PaintBackgroundLinha();
        }

        for (auto it = projectiles.begin(); it != projectiles.end();)
        {
            auto projectile = *it;
            projectile->Update();
            if (IsOutOfBounds(projectile->GetPosition())
                || (IsOfType<FriendlyProjectile>(projectile) && enemiesManager->CheckCollision(projectile->GetHitbox()))
                || (IsOfType<EnemyProjectile>(projectile) && player->CheckCollision(projectile->GetHitbox())))
            {
                projectile->OnHit();
                it = projectiles.erase(it);
            }
            else
            {
                ++it;
            }
        }

        this->enemiesManager->Update();
        this->player->Update();

        DrawGizmos();

        this->uiManager->SetPlayerScore(playerScore);
        this->uiManager->Update();
    }

    void EnemyDeathCallback(int enemyValue)
    {
        playerScore += enemyValue;
    }

    void KeyPressed(int key)
    {
        this->keyboardHandler->KeyPressed(key);
    }

    void KeyReleased(int key)
    {
        this->keyboardHandler->KeyReleased(key);
    }

private:
    KeyboardHandler *keyboardHandler = NULL;
    Player *player = NULL;
    BorderController *leftBorder = NULL;
    BorderController *rightBorder = NULL;
    EnemiesManager *enemiesManager = NULL;
    UIManager * uiManager = NULL;

    vector<Projectile *> projectiles;

    int playerScore = 0;

    int flag = 0;

    const int BorderWidth = 100;

    void DrawGizmos()
    {
        CV::color(1, 1, 1);

        for (auto projectile : projectiles)
        {
            CV::polygon(projectile->GetHitbox());
        }

        for (auto& row : this->enemiesManager->swarm)
        {
            for (auto& enemy : row)
            {
                if (enemy != nullptr)
                {
                    CV::polygon(enemy->GetHitbox());
                }
            }
        }

        CV::polygon(player->GetHitbox());
    }

    int lastH = 0;
    void PaintBackground()
    {
        for (int i = 0; i < leftBorder->points.size(); i++)
        {
            VectorHomo leftPoint = leftBorder->points[i];
            VectorHomo rightPoint = rightBorder->points[i];

            for (int h = lastH; h < leftPoint.y; h++)
            {
                for (int j = 0; j < ConstScreenWidth; j++)
                {
                    if (j < leftPoint.x || j > rightPoint.x)
                    {
                        CV::color(0.180, 0.220, 0.259);
                    }
                    else
                    {
                        CV::color(0.098, 0.125, 0.157);
                    }

                    CV::point(j, h);
                }
            }

            lastH = leftPoint.y;
        }
    }

    void PaintBackgroundLinha()
    {
        for (int i = 0; i < leftBorder->points.size(); i++)
        {
            VectorHomo leftPoint = leftBorder->points[i];
            VectorHomo rightPoint = rightBorder->points[i];

            for (int h = lastH; h < leftPoint.y; h++)
            {
                CV::color(0.180, 0.220, 0.259);
                CV::line(0, h, leftPoint.x, h);
                CV::line(rightPoint.x, h, ConstScreenWidth, h);

                CV::color(0.098, 0.125, 0.157);
                CV::line(leftPoint.x, h, rightPoint.x, h);
            }

            lastH = leftPoint.y;
        }
    }

    void InstantiatePlayerProjectile()
    {
        projectiles.push_back(new FriendlyProjectile(this->player->GetPosition()));
    }

    void InstantiateEnemyProjectile(VectorHomo position)
    {
        projectiles.push_back(new EnemyProjectile(position));
    }

    void SetKeyboardCallbacks()
    {
        this->keyboardHandler->RegisterCallbacks(119, bind(&Player::StartMovingUp, player), bind(&Player::StopMovingUp, player));       // W move pra cima
        this->keyboardHandler->RegisterCallbacks(201, bind(&Player::StartMovingUp, player), bind(&Player::StopMovingUp, player));       // UP move pra cima
        this->keyboardHandler->RegisterCallbacks(97, bind(&Player::StartMovingLeft, player), bind(&Player::StopMovingLeft, player));    // A move pra esquerda
        this->keyboardHandler->RegisterCallbacks(200, bind(&Player::StartMovingLeft, player), bind(&Player::StopMovingLeft, player));   // LEFT move pra esquerda
        this->keyboardHandler->RegisterCallbacks(115, bind(&Player::StartMovingDown, player), bind(&Player::StopMovingDown, player));   // S move pra baixo
        this->keyboardHandler->RegisterCallbacks(203, bind(&Player::StartMovingDown, player), bind(&Player::StopMovingDown, player));   // DOWN move pra baixo
        this->keyboardHandler->RegisterCallbacks(100, bind(&Player::StartMovingRight, player), bind(&Player::StopMovingRight, player)); // D move pra direita
        this->keyboardHandler->RegisterCallbacks(202, bind(&Player::StartMovingRight, player), bind(&Player::StopMovingRight, player)); // RIGHT move pra direita
        this->keyboardHandler->RegisterCallbacks(32, bind(&Player::StartShooting, player), bind(&Player::StopShooting, player));        // SPACE atira
    }
};

#endif // GAMEMANAGER_H_INCLUDED
