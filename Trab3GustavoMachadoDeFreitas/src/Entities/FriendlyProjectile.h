#ifndef FRIENDLYPROJECTILE_H_INCLUDED
#define FRIENDLYPROJECTILE_H_INCLUDED

#include "Projectile.h"

class FriendlyProjectile : public Projectile
{
public:
    FriendlyProjectile(VectorHomo transform) :
    Projectile(transform)
    {
        this->moveDirection = VectorHomo(0, 1);
        this->colorIndex = 2;
    }
protected:
};

#endif // FRIENDLYPROJECTILE_H_INCLUDED