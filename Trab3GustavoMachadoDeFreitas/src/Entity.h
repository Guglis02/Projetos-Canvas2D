#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "Vector2.h"

class Entity
{
public:
    Entity(Vector2 transform)
    {
        this->transform = transform;
    }

    Vector2 transform;

    void Move(Vector2 inc)
    {
        this->transform += (inc * moveSpeed);
    }

    virtual void Update(){}
protected:
    virtual void Render(){}

    float moveSpeed = 0;
};

#endif // ENTITY_H_INCLUDED
