#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "../Utils/VectorHomo.h"
#include "../Utils/FpsController.h"
#include "../Utils/CurveUtils.h"
#include "../Utils/PointsUtils.h"
#include "../gl_canvas2d.h"

enum EnemyState {
    Attacking,
    Roaming
};

class Enemy : public Entity
{
public:
    Enemy(VectorHomo transform) :
    Entity(transform)
    {
        state = Roaming;
        moveSpeed = 100;

        this->hitbox.push_back(VectorHomo(-32, -32));
        this->hitbox.push_back(VectorHomo(-32, 32));
        this->hitbox.push_back(VectorHomo(32, 32));
        this->hitbox.push_back(VectorHomo(32, -32));
    }

    void Update()
    {
        if (state == Roaming)
        {
            MoveToTarget();
        }

        if (DistanceBetweenTwoPoints(transform, targetPoint) < 1.0)
        {
            state = Attacking;
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
protected:
    EnemyState state;
    VectorHomo targetPoint;
    VectorHomo startingPoint;
    VectorHomo controlPoint1;
    VectorHomo controlPoint2;

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
        CV::translate(transform);

        CV::color(0.5f, 0.5f, 0.5f);

        // Asas
        CV::rectFill(VectorHomo(-32, 32),
                     VectorHomo(-28, -32));
        CV::rectFill(VectorHomo(32, 32),
                     VectorHomo(28, -32));

        // Hastes
        CV::rectFill(VectorHomo(-32, 5),
                     VectorHomo(0, -5));
        CV::rectFill(VectorHomo(32, 5),
                     VectorHomo(0, -5));

        CV::color(0.6f, 0.6f, 0.6f);
        CV::circleFill(0, 0, 16, 16);


        CV::translate(0, 0);
    }
};



#endif // ENEMY_H_INCLUDED
