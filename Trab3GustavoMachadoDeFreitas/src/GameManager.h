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
#include "Bmp/Bmp.h"

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

        this->player = new Player(VectorHomo(ConstScreenWidth >> 1, 200),
                                  bind(&GameManager::InstantiatePlayerProjectile, this),
                                  bind(&GameManager::PlayerDeathCallback, this));

        this->uiManager = new UIManager();
        this->uiManager->SetPlayerMaxHP(this->player->GetMaxHP());

        Bmp* upperTrenchBmp = new Bmp("./Trab3GustavoMachadoDeFreitas/images/UpperTrench.bmp");
        upperTrenchBmp->convertBGRtoRGB();
        this->upperTrenchTexture = upperTrenchBmp->getImage();
        Bmp* lowerTrenchBmp = new Bmp("./Trab3GustavoMachadoDeFreitas/images/LowerTrench.bmp");
        lowerTrenchBmp->convertBGRtoRGB();
        this->lowerTrenchTexture = lowerTrenchBmp->getImage();

        this->texturesWidth = upperTrenchBmp->getWidth();
        this->texturesHeight = upperTrenchBmp->getHeight();

        SetKeyboardCallbacks();
    }

    void Update()
    {
        FpsController::getInstance().updateFrames();

        leftBorder->Update(downSpeed);
        rightBorder->Update(downSpeed);

        if (qualityMode)
        {
            PaintBackgroundWithDots();
        }
        else
        {
            PaintBackgroundWithLines();
        }

        HandleProjectiles();
        HandlePlayerCollisions();

        this->enemiesManager->Update();
        this->player->Update();

        this->uiManager->SetPlayerScore(playerScore);
        this->uiManager->SetPlayerHP(this->player->GetHP());
        this->uiManager->Update();
    }

    void EnemyDeathCallback(int enemyValue)
    {
        playerScore += enemyValue;
    }

    void PlayerDeathCallback()
    {
        playerScore = 0;
        player->SetHP(player->GetMaxHP());
        player->SetPosition(VectorHomo(ConstScreenWidth >> 1, 200));
    }

    void KeyPressed(int key)
    {
        //printf("Key pressed: %d\n", key);
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

    const float downSpeed = 100.0;

    int texturesWidth;
    int texturesHeight;
    uchar* upperTrenchTexture = NULL;
    uchar* lowerTrenchTexture = NULL;

    vector<Projectile *> projectiles;

    int playerScore = 0;

    bool qualityMode = false;
    void ToggleQualityMode()
    {
        qualityMode = !qualityMode;
    }

    const int BorderWidth = 100;

    int lastH = 0;
    int textureIterator = 0;
    void PaintBackgroundWithDots()
    {

        if (lowerTrenchTexture == nullptr || upperTrenchTexture == nullptr)
        {
            return;
        }

        // Para cada ponto amostrado da borda
        for (int i = 0; i < leftBorder->points.size(); i++)
        {
            VectorHomo leftPoint = leftBorder->points[i];
            VectorHomo rightPoint = rightBorder->points[i];

            // Para cada linha entre o ponto anterior e o atual
            for (int h = lastH; h < leftPoint.y; h++)
            {
                // Para cada pixel da linha
                for (int j = 0; j < ConstScreenWidth*3; j+=3)
                {
                    textureIterator = textureIterator % texturesHeight;
                    int pos = textureIterator * ((3 * (texturesWidth + 1) / 4) * 4) + j;
                    int pixelX = j/3;

                    if (pixelX > leftPoint.x && pixelX < rightPoint.x)
                    {
                        CV::color(
                            (float)lowerTrenchTexture[pos] / 255.0,
                            (float)lowerTrenchTexture[pos + 1] / 255.0,
                            (float)lowerTrenchTexture[pos + 2] / 255.0);
                    }
                    else
                    {
                        CV::color(
                            (float)upperTrenchTexture[pos] / 255.0,
                            (float)upperTrenchTexture[pos + 1] / 255.0,
                            (float)upperTrenchTexture[pos + 2] / 255.0);
                    }

                    CV::point(pixelX, h);
                }
                textureIterator++;
            }

            lastH = leftPoint.y;
        }
    }

    void PaintBackgroundWithLines()
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

    void HandleProjectiles()
    {
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
    }

    void HandlePlayerCollisions()
    {
        if (player->GetPosition().x < ConstScreenWidth >> 1 &&
            (leftBorder->CheckCollision(player->GetHitbox())
            || player->GetPosition().x < BorderWidth))
        {
            player->OnHit();
        }
        if (player->GetPosition().x > ConstScreenWidth >> 1 &&
            (rightBorder->CheckCollision(player->GetHitbox())
            || player->GetPosition().x > ConstScreenWidth - BorderWidth))
        {
            player->OnHit();
        }
        if (enemiesManager->CheckCollision(player->GetHitbox()))
        {
            player->OnHit();
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
        this->keyboardHandler->RegisterCallbacks(113, nullptr, bind(&GameManager::ToggleQualityMode, this));        // Q Ativa/Desativa modo de qualidade
    }
};

#endif // GAMEMANAGER_H_INCLUDED
