#ifndef ENEMYPROJECTILE_H_INCLUDED
#define ENEMYPROJECTILE_H_INCLUDED

#include "Projectile.h"

class EnemyProjectile : public Projectile
{
public:
    EnemyProjectile(VectorHomo transform) :
    Projectile(transform)
    {
        this->moveDirection = VectorHomo(0, -1);
        this->colorIndex = 3;
    }
protected:
};

#endif // ENEMYPROJECTILE_H_INCLUDED