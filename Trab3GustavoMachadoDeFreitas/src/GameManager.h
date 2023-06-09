#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

#include "./Utils/KeyboardHandler.h"
#include "./Utils/VectorHomo.h"
#include "Entities/Player.h"
#include "Entities/EnemyProjectile.h"
#include "Entities/FriendlyProjectile.h"
#include "Entities/Enemy.h"
#include "./Utils/FpsController.h"
#include "./Utils/GlobalConsts.h"
#include "UIManager.h"
#include "EnemiesManager.h"
#include "GameState.h"
#include "CutscenesManager.h"
#include "BackgroundManager.h"

// Classe principal que comanda o funcionamento do jogo.
class GameManager
{
public:
    GameManager()
    {
        this->keyboardHandler = new KeyboardHandler();

        this->player = new Player(VectorHomo(ConstScreenWidth >> 1, 200),
                                  bind(&GameManager::InstantiatePlayerProjectile, this),
                                  bind(&GameManager::PlayerDeathCallback, this));

        this->enemiesManager = new EnemiesManager(BorderWidth);
        this->enemiesManager->SetCallbacks(bind(&GameManager::EnemyDeathCallback, this, placeholders::_1),
                                           bind(&GameManager::InstantiateEnemyProjectile, this, placeholders::_1));

        this->uiManager = new UIManager();
        this->uiManager->SetPlayerMaxHP(this->player->GetMaxHP());

        this->cutscenesManager = new CutscenesManager();

        this->backgroundManager = new BackgroundManager(BorderWidth);

        SetKeyboardCallbacks();
    }

    void Update()
    {
        FpsController::getInstance().updateFrames();

        if (gameState == GameState::GameOver)
        {
            this->cutscenesManager->GameOverCutscene();
            this->uiManager->DrawGameOverScreen();
            return;
        }

        if (gameState == GameState::Victory)
        {
            this->cutscenesManager->VictoryCutscene();
            this->uiManager->DrawGameOverScreen();
            return;
        }

        backgroundManager->Update(downSpeed);

        if (gameState == GameState::StartScreen)
        {
            this->uiManager->DrawStartScreen();
        }

        if (gameState == GameState::Endless || gameState == GameState::TrenchRun)
        {
            HandleProjectiles();
            HandlePlayerCollisions();

            this->enemiesManager->Update();
            this->player->Update();

            this->uiManager->SetPlayerScore(playerScore);
            this->uiManager->SetPlayerHP(this->player->GetHP());
        }

        this->uiManager->Update();
    }

    void StartEndlessMode()
    {
        gameState = GameState::Endless;
        ResetPlayer();
        ResetEntities();
    }

    void StartTrenchRun()
    {
        gameState = GameState::TrenchRun;
        ResetPlayer();
        ResetEntities();
    }

    // Quando um inimigo morre, soma o valor dele na pontuação total
    // caso o player esteja jogando o modo com final, verifica se a pontuação
    // já é suficiente para ganhar o jogo.
    void EnemyDeathCallback(int enemyValue)
    {
        playerScore += enemyValue;

        if (gameState == GameState::TrenchRun && playerScore >= TargetPoints)
        {
            gameState = GameState::Victory;
        }
    }

    void PlayerDeathCallback()
    {
        gameState = GameState::GameOver;
    }

    void KeyPressed(int key)
    {
        // printf("Key pressed: %d\n", key);
        this->keyboardHandler->KeyPressed(key);
    }

    void KeyReleased(int key)
    {
        this->keyboardHandler->KeyReleased(key);
    }

private:
    KeyboardHandler *keyboardHandler = NULL;
    Player *player = NULL;
    EnemiesManager *enemiesManager = NULL;
    UIManager *uiManager = NULL;
    CutscenesManager *cutscenesManager = NULL;
    BackgroundManager *backgroundManager = NULL;

    const float downSpeed = 100.0;
    const int BorderWidth = 100;

    GameState gameState = GameState::StartScreen;

    vector<Projectile *> projectiles;

    int playerScore = 0;

    // Verifica se algum projétil está fora da tela
    // ou colidiu com alguma entidade.
    void HandleProjectiles()
    {
        for (auto it = projectiles.begin(); it != projectiles.end();)
        {
            auto projectile = *it;
            projectile->Update();
            if (IsOutOfBounds(projectile->GetPosition()) || (IsOfType<FriendlyProjectile>(projectile) && enemiesManager->CheckCollision(projectile->GetHitbox())) || (IsOfType<EnemyProjectile>(projectile) && player->CheckCollision(projectile->GetHitbox())))
            {
                projectile->OnHit();
                it = projectiles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // Checa colisões do player com as bordas e com os inimigos
    void HandlePlayerCollisions()
    {
        if (player->GetPosition().x < ConstScreenWidth >> 1 &&
            (backgroundManager->CheckLeftCollision(player->GetHitbox()) || player->GetPosition().x < BorderWidth))
        {
            player->OnHit();
            return;
        }
        if (player->GetPosition().x > ConstScreenWidth >> 1 &&
            (backgroundManager->CheckRightCollision(player->GetHitbox()) || player->GetPosition().x > ConstScreenWidth - BorderWidth))
        {
            player->OnHit();
            return;
        }
        if (enemiesManager->CheckCollision(player->GetHitbox()))
        {
            player->OnHit();
            return;
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

    void ResetPlayer()
    {
        playerScore = 0;
        player->SetHP(player->GetMaxHP());
        player->SetPosition(VectorHomo(ConstScreenWidth >> 1, 200));
    }

    void ResetEntities()
    {
        this->projectiles.clear();
        this->enemiesManager->Reset();
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
        this->keyboardHandler->RegisterCallbacks(113, nullptr, bind(&BackgroundManager::ToggleQualityMode, backgroundManager));         // Q Ativa/Desativa modo de qualidade
        this->keyboardHandler->RegisterCallbacks(101, nullptr, bind(&GameManager::StartEndlessMode, this));                             // E Ativa modo endless
        this->keyboardHandler->RegisterCallbacks(116, nullptr, bind(&GameManager::StartTrenchRun, this));                               // T Ativa o modo trench run
    }
};

#endif // GAMEMANAGER_H_INCLUDED
