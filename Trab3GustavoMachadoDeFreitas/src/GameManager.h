#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include "VectorHomo.h"
#include "BorderController.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Entities/Enemy.h"
#include "FpsController.h"

class GameManager
{
public:
    GameManager(int screenWidth, int screenHeight)
    {
        this->mouseHandler = new MouseHandler();
        this->keyboardHandler = new KeyboardHandler();
        this->leftBorder = new BorderController(150, screenWidth, screenHeight);
        this->rightBorder = new BorderController(screenWidth - 150, screenWidth, screenHeight);
        this->player = new Player(VectorHomo(100,100), bind(&GameManager::InstantiatePlayerProjectile, this));
        SetKeyboardCallbacks();
    }

    MouseHandler* mouseHandler = NULL;
    KeyboardHandler* keyboardHandler = NULL;
    Player* player = NULL;
    BorderController* leftBorder = NULL;
    BorderController* rightBorder = NULL;

    vector<Projectile*> friendlyProjectiles;
    vector<Enemy*> enemies;

    int screenWidth;
    int screenHeight;
    float enemyCooldown = 10.0;
    float timeSinceLastEnemy = enemyCooldown;

    void Update(int screenWidth, int screenHeight)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        leftBorder->Update(100.0);
        rightBorder->Update(100.0);

        PaintBackground();

        FpsController::getInstance().updateFrames();
        char fpsLabel[64];
        sprintf(fpsLabel, "%.1f", FpsController::getInstance().getFps());
        CV::color(2);
        CV::text(50, screenHeight - 50, fpsLabel);

        for (auto projectile : friendlyProjectiles)
        {
            projectile->Update();
        }

        HandleEnemies();

        this->player->Update();

        DrawGizmos();
    }

    void DrawGizmos()
    {
        CV::color(1,1,1);
        
        for (auto projectile : friendlyProjectiles)
        {
            CV::polygon(projectile->GetHitbox());
        }

        for (auto enemy : enemies)
        {
            CV::polygon(enemy->GetHitbox());
        }

        CV::polygon(player->GetHitbox());
    }
    
    // int lastH = 0;
    // void PaintBackground()
    // {
    //     for (int i = 0; i < leftBorder->points.size(); i++)
    //     {
    //         VectorHomo leftPoint = leftBorder->points[i];
    //         VectorHomo rightPoint = rightBorder->points[i];

    //         for (int h = lastH; h < leftPoint.y; h++)
    //         {
    //             for (int j = 0; j < screenWidth; j++)
    //             {
    //                 if (j < leftPoint.x || j > rightPoint.x)
    //                 {
    //                     CV::color(1,0.5,0.5);
    //                 }
    //                 else
    //                 {
    //                     CV::color(0.5,1,0.5);
    //                 }

    //                 CV::point(j, h);
    //             }
    //         }

    //         lastH = leftPoint.y;
    //     }
    // }
    int lastH = 0;
    void PaintBackground()
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

    void InstantiateEnemy()
    {
        enemies.push_back(new Enemy(this->player->GetPosition() + VectorHomo(0, screenHeight - 100)));
    }

    void HandleEnemies()
    {
        timeSinceLastEnemy += FpsController::getInstance().GetDeltaTime();

        if (timeSinceLastEnemy >= enemyCooldown)
        {
            InstantiateEnemy();
            timeSinceLastEnemy = 0;
        }

        for (auto enemy : enemies)
        {
            enemy->Update();
        }
    }

    void SetKeyboardCallbacks()
    {
        this->keyboardHandler->RegisterCallbacks(119, bind(&Player::StartMovingUp, player), bind(&Player::StopMovingUp, player)); // W move pra cima
        this->keyboardHandler->RegisterCallbacks(201, bind(&Player::StartMovingUp, player), bind(&Player::StopMovingUp, player)); // UP move pra cima
        this->keyboardHandler->RegisterCallbacks(97, bind(&Player::StartMovingLeft, player), bind(&Player::StopMovingLeft, player)); // A move pra esquerda
        this->keyboardHandler->RegisterCallbacks(200, bind(&Player::StartMovingLeft, player), bind(&Player::StopMovingLeft, player)); // LEFT move pra esquerda
        this->keyboardHandler->RegisterCallbacks(115, bind(&Player::StartMovingDown, player), bind(&Player::StopMovingDown, player)); // S move pra baixo
        this->keyboardHandler->RegisterCallbacks(203, bind(&Player::StartMovingDown, player), bind(&Player::StopMovingDown, player)); // DOWN move pra baixo
        this->keyboardHandler->RegisterCallbacks(100, bind(&Player::StartMovingRight, player), bind(&Player::StopMovingRight, player)); // D move pra direita
        this->keyboardHandler->RegisterCallbacks(202, bind(&Player::StartMovingRight, player), bind(&Player::StopMovingRight, player)); // RIGHT move pra direita
        this->keyboardHandler->RegisterCallbacks(32, bind(&Player::StartShooting, player), bind(&Player::StopShooting, player)); // SPACE atira
    }

    void KeyPressed(int key)
    {
        this->keyboardHandler->KeyPressed(key);
    }

    void KeyReleased(int key)
    {
        this->keyboardHandler->KeyReleased(key);
    }
};

#endif // GAMEMANAGER_H_INCLUDED
