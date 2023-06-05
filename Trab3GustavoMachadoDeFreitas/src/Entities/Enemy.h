#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(VectorHomo transform) :
    Entity(transform)
    {
        moveSpeed = 100;        
    }
    
    void Update()
    {
        Move(VectorHomo(0, -1));

        Render();
    }
protected:
    void Render()
    {
        CV::color(4);
        CV::circleFill(transform.x, transform.y, 20, 20);
    }
};

#endif // ENEMY_H_INCLUDED