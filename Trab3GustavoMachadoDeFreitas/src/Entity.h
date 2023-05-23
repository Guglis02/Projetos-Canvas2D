#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "VectorHomo.h"
#include "Matrix.h"

class Entity
{
public:
    Entity(VectorHomo transform)
    {
        this->transform = transform;
        this->matrix = new Matrix();
    }

    virtual void Update(){}
protected:
    virtual void Render(){}

    float moveSpeed = 0;

    VectorHomo transform;
    Matrix* matrix = NULL;
 
    void Move(VectorHomo inc)
    {
        matrix->BuildIdentity();
        matrix->Translation(inc.x, inc.y);
        this->transform = *matrix * this->transform;
        matrix->Reset();
    }
};

#endif // ENTITY_H_INCLUDED
