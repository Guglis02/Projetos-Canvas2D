#ifndef POWERUPSMANAGER_H_INCLUDED
#define POWERUPSMANAGER_H_INCLUDED

#include "./Entities/PowerUp.h"
#include "./Utils/GlobalConsts.h"

#include <vector>

using namespace std;

// Classe responsável por gerenciar os powerups do jogo
class PowerUpsManager
{
public:
    PowerUpsManager(int borderWidth)
    {
        this->borderWidth = borderWidth;
        this->spawnableArea = ConstScreenWidth - (borderWidth * 3);
    }

    void SetCallbacks(function<void()> shieldCallback,
                      function<void()> missileCallback,
                      function<void()> hpCallback)
    {
        this->shieldCallback = shieldCallback;
        this->missileCallback = missileCallback;
        this->hpCallback = hpCallback;
    }

    void Update()
    {
        timeSinceLast += FpsController::getInstance().GetDeltaTime();

        if (timeSinceLast >= SpawnCooldown)
        {
            InstantiatePowerUp();
            timeSinceLast = 0;
        }

        for (auto powerup : powerUps)
        {
            powerup->Update();
        }
    }

    void Reset()
    {
        for (auto powerup : powerUps)
        {
            delete powerup;
        }
    }

    bool CheckCollision(vector<VectorHomo> hitbox)
    {
        for (auto it = powerUps.begin(); it != powerUps.end(); ++it)
        {
            if ((*it)->CheckCollision(hitbox))
            {
                (*it)->buffCallback();
                delete *it;
                powerUps.erase(it);
                return true;
            }
        }

        return false;
    }

    vector<PowerUp *> powerUps;

private:
    float SpawnCooldown = 5;
    float timeSinceLast = SpawnCooldown;

    int borderWidth;
    int spawnableArea;

    function<void()> shieldCallback;
    function<void()> missileCallback;
    function<void()> hpCallback;

    void InstantiatePowerUp()
    {
        float spawnX = rand() % spawnableArea + (1.5 * borderWidth);
        float spawnY = ConstScreenHeight + 100;

        // Escolhe aleatoriamente o tipo de powerup
        int powerUpType = rand() % 3;

        switch (powerUpType)
        {
            case 0:
                powerUps.push_back(new PowerUp(VectorHomo(spawnX, spawnY), "S", shieldCallback));
                break;
            case 1:
                powerUps.push_back(new PowerUp(VectorHomo(spawnX, spawnY), "M", missileCallback));
                break;
            case 2:
                powerUps.push_back(new PowerUp(VectorHomo(spawnX, spawnY), "HP", hpCallback));
                break;
            default:
                break;
        }

    }
};

#endif // POWERUPSMANAGER_H_INCLUDED