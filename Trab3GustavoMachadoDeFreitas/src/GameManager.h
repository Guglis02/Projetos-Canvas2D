#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include "VectorHomo.h"
#include "Entities/Player.h"
#include "Entities/Projectile.h"
#include "Entities/Enemy.h"
#include "FpsController.h"

class GameManager
{
public:
    GameManager()
    {
        this->mouseHandler = new MouseHandler();
        this->keyboardHandler = new KeyboardHandler();
        this->player = new Player(VectorHomo(100,100), bind(&GameManager::InstantiatePlayerProjectile, this));
        SetKeyboardCallbacks();
    }

    MouseHandler* mouseHandler = NULL;
    KeyboardHandler* keyboardHandler = NULL;
    Player* player = NULL;

    vector<Projectile*> friendlyProjectiles;
    vector<Enemy*> enemies;

    void Update(int screenWidth, int screenHeight)
    {
        CV::translate(0, 0);

        FpsController::getInstance().getFrames();
        char fpsLabel[64];
        sprintf(fpsLabel, "%.1f", FpsController::getInstance().getFps());
        CV::color(2);
        CV::text(50, screenHeight - 50, fpsLabel);
        this->player->Update();

        for (auto projectile : friendlyProjectiles)
        {
            projectile->Update();
        }

        CV::translate(0, -1 * this->player->GetPosition().y);
    }

    void InstantiatePlayerProjectile()
    {
        friendlyProjectiles.push_back(new Projectile(this->player->GetPosition()));
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
