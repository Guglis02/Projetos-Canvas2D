#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include "VectorHomo.h"
#include "Player.h"
#include "FpsController.h"

class GameManager
{
public:
    GameManager()
    {
        this->mouseHandler = new MouseHandler();
        this->keyboardHandler = new KeyboardHandler();
        this->player = new Player(VectorHomo(100,100));
        SetKeyboardCallbacks();
    }

    MouseHandler* mouseHandler = NULL;
    KeyboardHandler* keyboardHandler = NULL;
    Player* player = NULL;

    void Update(int screenWidth, int screenHeight)
    {
        char fpsLabel[64];
        sprintf(fpsLabel, "%.1f", FpsController::getInstance().getFrames());
        CV::color(2);
        CV::text(50, screenHeight - 50, fpsLabel);
        this->player->Update();
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
    }


    void KeyPressed(int key)
    {
        this->keyboardHandler->KeyPressed(key);
        // switch (key)
        // {
        // case 27:
        // case 8:
        //     printf("\nBack");
        //     break;
        // case 32:
        // case 13:
        //     printf("\nConfirm");
        //     break;
        // default:
        //     break;
        // }
    }

    void KeyReleased(int key)
    {
        this->keyboardHandler->KeyReleased(key);
    }
};

#endif // GAMEMANAGER_H_INCLUDED
