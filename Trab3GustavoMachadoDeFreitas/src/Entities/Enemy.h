#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"

enum class EnemyState {
    Attacking,
    Roaming
};

class Enemy : public Entity
{
public:
    Enemy(VectorHomo transform) :
    Entity(transform)
    {
        moveSpeed = 100; 

        this->hitbox.push_back(VectorHomo(-32, -32));
        this->hitbox.push_back(VectorHomo(-32, 32));
        this->hitbox.push_back(VectorHomo(32, 32));
        this->hitbox.push_back(VectorHomo(32, -32));       
    }
    
    void Update()
    {
        //Move(VectorHomo(0, 0));

        Render();
    }
protected:
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