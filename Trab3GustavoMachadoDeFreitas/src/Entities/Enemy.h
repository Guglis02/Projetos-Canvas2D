#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "./GuidedProjectile.h"
#include "../Utils/VectorHomo.h"
#include "../Utils/FpsController.h"
#include "../Utils/CurveUtils.h"
#include "../Utils/PointsUtils.h"
#include "../Utils/VectorArts.h"
#include "../Utils/TrajectoryInterpolator.h"
#include "../gl_canvas2d.h"
#include <vector>

using namespace std;

// Estados em que um inimigo pode estar
enum EnemyState
{
    InPlace,
    Roaming
};

// Classe base para todos os inimigos do jogo
class Enemy : public Entity
{
public:
    Enemy(VectorHomo transform,
          VectorHomo swarmPosition,
          function<void(int)> deathCallback,
          function<void(VectorHomo)> shootCallback)
        : Entity(transform)
    {
        state = Roaming;
        moveSpeed = 100;
        this->deathCallback = deathCallback;
        this->shootCallback = shootCallback;

        trajectoryInterpolator = new TrajectoryInterpolator(transform, swarmPosition);

        this->hitbox.push_back(VectorHomo(-32, -32));
        this->hitbox.push_back(VectorHomo(-32, 32));
        this->hitbox.push_back(VectorHomo(32, 32));
        this->hitbox.push_back(VectorHomo(32, -32));
    }
    ~Enemy()
    {
        delete trajectoryInterpolator;
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

    void OnHit()
    {
        deathCallback(pointValue);
        Entity::OnHit();
    }

    bool IsInPlace()
    {
        return state == InPlace;
    }

protected:
    EnemyState state;
    TrajectoryInterpolator* trajectoryInterpolator;

    // Valor de pontos que o player ganha matando este inimigo
    int pointValue = 10;
    function<void(int)> deathCallback;

    float shootCooldown = 3;
    float timeSinceLastShot = shootCooldown;
    // O quão abaixo do inimigo o tiro deve spawnar
    VectorHomo shotOffset = VectorHomo(0, -16);

    function<void(VectorHomo)> shootCallback;

    // Interpola posição do inimigo através de uma curva de Bezier
    void MoveToTarget()
    {
        trajectoryInterpolator->Update();

        transform = trajectoryInterpolator->GetCurrentPosition();

        if (trajectoryInterpolator->IsFinished())
        {
            state = InPlace;
        }
    }

    void HandleShooting()
    {
        timeSinceLastShot += FpsController::getInstance().GetDeltaTime();

        // Existe uma chance aleatória do inimigo atirar
        if (rand() % 500 == 1 && (timeSinceLastShot >= shootCooldown))
        {
            shootCallback(transform + shotOffset);
            timeSinceLastShot = 0;
        }
    }

    void Render()
    {
        DrawTieFighter(transform);
    }
};

#endif // ENEMY_H_INCLUDED
