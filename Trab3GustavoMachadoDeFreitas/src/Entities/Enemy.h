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

        if (DistanceBetweenTwoPoints(transform, targetPoint) < 1.0)
        {
            state = InPlace;
        }

        timeSinceLastShot += FpsController::getInstance().GetDeltaTime();

        // Existe uma chance aleatória do inimigo atirar 
        if (rand() % 500 == 1 && (timeSinceLastShot >= shootCooldown))
        {
            shootCallback(transform);
            timeSinceLastShot = 0;
        }

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

    int const pointValue = 10;
    function<void(int)> deathCallback;

    float shootCooldown = 3;
    float timeSinceLastShot = shootCooldown;

    function<void(VectorHomo)> shootCallback;

    void MoveToTarget()
    {
        // float t = 1.0 / DistanceBetweenTwoPoints(transform, targetPoint);
        // printf("t: %f\n", t);

        // for (float i = 0; i <= 1; i += 0.01)
        // {
        //     VectorHomo p = Bezier3(startingPoint, controlPoint1, controlPoint2, targetPoint, i);
        //     CV::color(4);
        //     CV::point(p.x, p.y);
        // }

        // transform = Bezier3(startingPoint, controlPoint1, controlPoint2, targetPoint, t);

        VectorHomo direction = targetPoint - startingPoint;
        direction.normalize();
        // CV::line(transform, transform + direction * 100);

        Move(direction);
    }

    void Render()
    {
        DrawTieFighter(transform);
    }
};

#endif // ENEMY_H_INCLUDED