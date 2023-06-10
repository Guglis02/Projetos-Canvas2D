#ifndef BOMBERENEMY_H_INCLUDED
#define BOMBERENEMY_H_INCLUDED

#include "Enemy.h"

// Variante de inimigo com dois canhões que disparam em intervalos diferentes
// e vale mais pontos
class BomberEnemy : public Enemy
{
public:
    BomberEnemy(VectorHomo transform,
                VectorHomo swarmPosition,
                function<void(int)> deathCallback,
                function<void(VectorHomo)> shootCallback)
        : Enemy(transform, swarmPosition, deathCallback, shootCallback)
    {
        pointValue = 20;
    }
    ~BomberEnemy()
    {
    }

    void Update()
    {
        if (state == Roaming)
        {
            MoveToTarget();
        }

        HandleShooting();

        Render();
    }

protected:
    float timeSinceLastLeftShot = shootCooldown;
    float timeSinceLastRightShot = shootCooldown;
    VectorHomo cannonDistance = VectorHomo(10, 0);

    void Render()
    {
        DrawTieBomber(transform);
    }

    void HandleShooting()
    {
        timeSinceLastLeftShot += FpsController::getInstance().GetDeltaTime();
        timeSinceLastRightShot += FpsController::getInstance().GetDeltaTime();

        // Os canhões esquerdo e direito possuem chances diferetes de disparar
        if ((timeSinceLastLeftShot >= shootCooldown) && rand() % 500 == 1)
        {
            shootCallback(transform + shotOffset - cannonDistance);
            timeSinceLastLeftShot = 0;
        }

        if ((timeSinceLastRightShot >= shootCooldown) && rand() % 300 == 1)
        {
            shootCallback(transform + shotOffset + cannonDistance);
            timeSinceLastRightShot = 0;
        }
    }
};

#endif // BOMBERENEMY_H_INCLUDED
