#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "MouseHandler.h"
#include "Vector2.h"
#include "Player.h"

class GameManager
{
public:
    GameManager()
    {
        this->mouseHandler = new MouseHandler();
        this->player = new Player(Vector2(100,100));
    }

    MouseHandler* mouseHandler = NULL;
    Player* player = NULL;

    void Update()
    {
        this->player->Update();
    }

    void KeyPressed(int key)
    {
        switch (key)
        {
        case 119:
        case 201:
            player->isMovingUp = true;
            break;
        case 97:
        case 200:
            player->isMovingLeft = true;
            break;
        case 115:
        case 203:
            player->isMovingDown = true;
            break;
        case 100:
        case 202:
            player->isMovingRight = true;
            break;
        case 27:
        case 8:
            printf("\nBack");
            break;
        case 32:
        case 13:
            printf("\nConfirm");
            break;
        default:
            break;
        }
    }

    void KeyReleased(int key)
    {
        switch (key)
        {
        case 119:
        case 201:
            player->isMovingUp = false;
            player->movementDirection.y = 0;
            break;
        case 97:
        case 200:
            player->isMovingLeft = false;
            player->movementDirection.x = 0;
            break;
        case 115:
        case 203:
            player->isMovingDown = false;
            player->movementDirection.y = 0;
            break;
        case 100:
        case 202:
            player->isMovingRight = false;
            player->movementDirection.x = 0;
            break;
        default:
            break;
        }
    }
};

#endif // GAMEMANAGER_H_INCLUDED
