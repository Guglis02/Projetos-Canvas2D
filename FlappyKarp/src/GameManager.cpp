#include "GameManager.h"
#include "gl_canvas2d.h"
#include "Player.h"
#include "Obstacle.h"
#include <ctime>

GameManager::GameManager(int screenWidth, int screenHeight)
{
    this->gameState = StartScreen;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    player = new Player(screenWidth/3, screenHeight/2);

    oldTime = clock();
    deltaTime = 0;
}

void GameManager::Update()
{
    CalculateDeltaTime();

    switch (gameState)
    {
        case StartScreen:
            color(2);
            player->Draw();
            color(3);
            text(2 * screenWidth / 5, screenHeight / 2, "Press Space");
            break;
        case Running:
            player->Draw();
            player->UpdateSpeedAndPosition(deltaTime);
            ObstacleManager();
            break;
        case GameOver:
            break;
    }
}

void GameManager::StartGame()
{
    // Seta o player
    // Seta o spawner de obstaculos
    // Troca o modo de jogo
    gameState = Running;
    player->Flap();
}

void GameManager::CalculateDeltaTime()
{
    deltaTime = clock() - oldTime;
    oldTime = clock();
}

void GameManager::ObstacleManager()
{
    // Isso aqui pode entrar no construtor de uma eventual classe ObstacleManager
    if (obstacles.size() == 0)
    {
        srand(time(NULL));

        int spaceBetweenObstacles = 200;
        for (int i = 0; i < maxObstacles; i++)
        {
            obstacles.push_back(Obstacle(screenWidth + spaceBetweenObstacles, screenHeight, rand() % 451));
            spaceBetweenObstacles += 200;
        }
    }

    for (int i = 0; i < obstacles.size(); i++)
    {
        obstacles[i].Update(deltaTime);

        if(obstacles[i].x <= 0)
        {
            printf("\nSaiu da tela");

            /*
            obstacles.erase(obstacles.begin() + i);
            i--;
            obstacles.push_back(Obstacle(screenWidth + 40, screenHeight, rand() % 451));*/
        }
    }
}

/* ////////////////////////////////////////////////////
   Funções responsáveis por tratar os inputs do usuário
   //////////////////////////////////////////////////// */
void GameManager::HandleInputs(int key)
{
    switch (gameState)
    {
        case StartScreen:
            HandleStartScreenInputs(key);
            break;
        case Running:
            HandleGameInputs(key);
            break;
        default:
            break;
    }
}

void GameManager::HandleStartScreenInputs(int key)
{
    switch (key)
    {
        case 32: // Tecla Espaço -> Inicia o jogo
            StartGame();
            break;
        default:
            break;
    }
}

void GameManager::HandleGameInputs(int key)
{
    switch (key)
    {
        case 32: // Tecla Espaço -> Player bate asas
            player->Flap();
            break;
        default:
            break;
    }
}
