/*
Autor: Gustavo Machado de Freitas

Classe GameManager:
    Responsável por gerenciar o jogo.
*/

#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__
#include <functional>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "gl_canvas2d.h"
#include "Player.h"
#include "Obstacle.h"

using namespace std;

class GameManager
{
    public:
        /*Construtor*/
        GameManager(int screenWidth, int screenHeight);

        void Update();
        void HandleInputs(int key);

    private:
        Player* player = NULL;

        const int maxObstacles = 3;
        vector<Obstacle> obstacles;

        enum gameStates
        {
            StartScreen,
            Running,
            GameOver
        };

        gameStates gameState;

        int screenWidth;
        int screenHeight;

        void StartGame();

        float oldTime;
        float deltaTime;
        void CalculateDeltaTime();

        const float obstacleCooldown = 3;
        float timeSinceLastObstacle = 0;
        void ObstacleManager();

        void HandleStartScreenInputs(int key);
        void HandleGameInputs(int key);
};

#endif

