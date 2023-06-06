#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#include "../gl_canvas2d.h"
#include "Entity.h"

class Projectile : public Entity
{
public:
    Projectile(VectorHomo transform) :
    Entity(transform)
    {
        this->moveSpeed = 1000;        
        
        this->hitbox.push_back(VectorHomo(-5, 0));
        this->hitbox.push_back(VectorHomo(-5, 25));
        this->hitbox.push_back(VectorHomo(5, 25));
        this->hitbox.push_back(VectorHomo(5, 0));
    }

    void Update()
    {
        Move(VectorHomo(0, 1));

        Render();
    }
protected:
    void Render()
    {
        CV::color(3);
        CV::rectFill(transform.x - 5, transform.y, transform.x + 5, transform.y + 25);
    }
};

#endif // PROJECTILE_H_INCLUDED
