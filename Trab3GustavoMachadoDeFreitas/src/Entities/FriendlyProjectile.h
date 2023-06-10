#ifndef FRIENDLYPROJECTILE_H_INCLUDED
#define FRIENDLYPROJECTILE_H_INCLUDED

#include "Projectile.h"

// Classe que representa um projétil amigo
class FriendlyProjectile : public Projectile
{
public:
    FriendlyProjectile(VectorHomo transform) : Projectile(transform)
    {
        this->moveDirection = VectorHomo(0, 1);
        this->colorIndex = 2;
    }
    ~FriendlyProjectile() {}

protected:
};

#endif // FRIENDLYPROJECTILE_H_INCLUDED