#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "../Utils/VectorHomo.h"
#include "../Utils/FpsController.h"
#include "../Utils/CurveUtils.h"
#include "../Utils/PointsUtils.h"
#include "../Utils/VectorArts.h"
#include "../gl_canvas2d.h"

enum EnemyState {
    InPlace,
    Roaming
};

class Enemy : public Entity
{
public:
    Enemy(VectorHomo transform,
        function<void(int)> deathCallback,
        function<void(VectorHomo)> shootCallback) :
    Entity(transform)
    {
        state = Roaming;
        moveSpeed = 100;
        this->deathCallback = deathCallback;
        this->shootCallback = shootCallback;

        this->hitbox.push_back(VectorHomo(-32, -32));
        this->hitbox.push_back(VectorHomo(-32, 32));
        this->hitbox.push_back(VectorHomo(32, 32));
        this->hitbox.push_back(VectorHomo(32, -32));
    }
    ~Enemy()
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

    void SetupRoaming(VectorHomo startingPoint, VectorHomo target, VectorHomo controlPoint1, VectorHomo controlPoint2)
    {
        this->startingPoint = startingPoint;
        this->targetPoint = target;
        this->controlPoint1 = controlPoint1;
        this->controlPoint2 = controlPoint2;
    }

    void OnHit()
    {
        deathCallback(pointValue);
        Entity::OnHit();
    }
protected:
    EnemyState state;
    VectorHomo targetPoint;
    VectorHomo startingPoint;
    VectorHomo controlPoint1;
    VectorHomo controlPoint2;

    int pointValue = 10;
    function<void(int)> deathCallback;

    float shootCooldown = 3;
    float timeSinceLastShot = shootCooldown;
    VectorHomo shotOffset = VectorHomo(0, -16);

    function<void(VectorHomo)> shootCallback;

    float t = 0;
    void MoveToTarget()
    {
        t += FpsController::getInstance().GetDeltaTime() * 0.5;

        transform = Bezier3(startingPoint, controlPoint1, controlPoint2, targetPoint, t);

        if (t >= 1)
        {
            transform = Bezier3(startingPoint, controlPoint1, controlPoint2, targetPoint, 1);
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
