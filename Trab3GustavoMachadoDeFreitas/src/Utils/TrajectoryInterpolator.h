#ifndef TRAJECTORYINTERPOLATOR_H_INCLUDED
#define TRAJECTORYINTERPOLATOR_H_INCLUDED

#include "../Utils/VectorHomo.h"
#include "../Utils/FpsController.h"
#include "../Utils/CurveUtils.h"
#include "../Utils/PointsUtils.h"
#include <math.h>

// Classe que interpola uma trajetória sob uma curva de Bezier
class TrajectoryInterpolator
{
public:
    TrajectoryInterpolator(
        VectorHomo startingPoint,
        VectorHomo target,
        float interpolationSpeed)
    {
        this->start = startingPoint;
        this->target = target;
        this->control1 = VectorHomo((target.x + start.x) / 2, start.y);
        this->control2 = VectorHomo((target.x + start.x) / 2, target.y);
        this->interpolationSpeed = interpolationSpeed;
    }

    // Atualiza a posição atual da trajetória
    void Update()
    {
        t += FpsController::getInstance().GetDeltaTime() * interpolationSpeed;
        t = min(t, 1.0f);

        if (t == 1)
        {
            isFinished = true;
        }

        currentPosition = Bezier3(start, control1, control2, target, t);
        VectorHomo derivative = Bezier3Derivative(start, control1, control2, target, t);
        angle = GetAngleWithXAxis(derivative);
        derivative.normalize();
    }

    void ChangeTarget(VectorHomo newTarget)
    {
        this->target = newTarget;
    }

    VectorHomo GetCurrentPosition()
    {
        return currentPosition;
    }

    float GetAngle()
    {
        return angle;
    }

    bool IsFinished()
    {
        return isFinished;
    }
protected:
    VectorHomo start;
    VectorHomo target;
    VectorHomo control1;
    VectorHomo control2;
    float interpolationSpeed = 0.5f;

    VectorHomo currentPosition;
    float t = 0;
    float angle = 0;
    bool isFinished = false;
};

#endif // ENEMY_H_INCLUDED
