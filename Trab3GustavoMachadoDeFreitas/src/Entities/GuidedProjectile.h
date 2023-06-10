#ifndef GUIDEDROJECTILE_H_INCLUDED
#define GUIDEDROJECTILE_H_INCLUDED

#include "FriendlyProjectile.h"
#include "../Utils/TrajectoryInterpolator.h"
#include "./Enemy.h"

// Classe que representa um projétil teleguiado
class GuidedProjectile : public FriendlyProjectile
{
public:
    GuidedProjectile(VectorHomo transform, Enemy* target) : FriendlyProjectile(transform)
    {
        this->target = target;
        this->trajectoryInterpolator = new TrajectoryInterpolator(transform, target->GetPosition(), 1.5);
        this->colorIndex = 2;
    }
    ~GuidedProjectile()
    {
        delete trajectoryInterpolator;
    }

    // A cada frame atualiza o interpolador pra posição atual do alvo
    void Update()
    {
        trajectoryInterpolator->ChangeTarget(target->GetPosition());
        trajectoryInterpolator->Update();
        angle = trajectoryInterpolator->GetAngle();
        transform = trajectoryInterpolator->GetCurrentPosition();

        timeCounter += FpsController::getInstance().GetDeltaTime();

        if (timeCounter >= 3.0f)
        {
            shouldDie = true;
        }

        Render();
    }

    bool ShouldDie()
    {
        return shouldDie;
    }

protected:
    TrajectoryInterpolator *trajectoryInterpolator;
    Enemy *target;
    float angle = 0;
    float timeCounter = 0.0f;
    bool shouldDie = false;

    void Render()
    {
        CV::color(colorIndex);
        CV::ellipsisFill(transform.x, transform.y, 5, 12, 32, angle + (PI * 0.5));
    }
};

#endif // GUIDEDROJECTILE_H_INCLUDED
