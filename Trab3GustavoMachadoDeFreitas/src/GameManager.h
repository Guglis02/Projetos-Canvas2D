#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "./Utils/KeyboardHandler.h"
#include "./Utils/VectorHomo.h"
#include "BorderController.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Entities/Enemy.h"
#include "./Utils/FpsController.h"
#include "EnemiesManager.h"

class GameManager
{
public:
    GameManager(int screenWidth, int screenHeight)
    {
        this->keyboardHandler = new KeyboardHandler();
        this->leftBorder = new BorderController(BorderWidth, screenWidth, screenHeight);
        this->rightBorder = new BorderController(screenWidth - BorderWidth, screenWidth, screenHeight);
        this->enemiesManager = new EnemiesManager(screenWidth, screenHeight, BorderWidth);
        this->player = new Player(VectorHomo(100, 100), bind(&GameManager::InstantiatePlayerProjectile, this));
        SetKeyboardCallbacks();
    }

    void Update(int screenWidth, int screenHeight)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

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

        FpsController::getInstance().updateFrames();
        char fpsLabel[64];
        sprintf(fpsLabel, "%.1f", FpsController::getInstance().getFps());
        CV::color(2);
        CV::text(50, screenHeight - 50, fpsLabel);

        for (auto projectile : friendlyProjectiles)
        {
            projectile->Update();
        }

        this->enemiesManager->Update();
        this->player->Update();

        DrawGizmos();
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

    vector<Projectile *> friendlyProjectiles;

    int screenWidth;
    int screenHeight;
    int flag = 0;

    const int BorderWidth = 150;

    void DrawGizmos()
    {
        CV::color(1, 1, 1);

        for (auto projectile : friendlyProjectiles)
        {
            CV::polygon(projectile->GetHitbox());
        }

        for (auto& row : this->enemiesManager->swarm)
        {
            for (auto& enemy : row)
            {
                if (enemy != nullptr)
                {
                    enemy->Update();
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
                for (int j = 0; j < screenWidth; j++)
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
                CV::line(rightPoint.x, h, screenWidth, h);

                CV::color(0.098, 0.125, 0.157);
                CV::line(leftPoint.x, h, rightPoint.x, h);
            }

            lastH = leftPoint.y;
        }
    }

    void InstantiatePlayerProjectile()
    {
        friendlyProjectiles.push_back(new Projectile(this->player->GetPosition()));
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
