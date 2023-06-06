#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "../VectorHomo.h"
#include "../Matrix.h"
#include "../FpsController.h"
#include <vector>

using namespace std;

class Entity
{
public:
    Entity(VectorHomo transform)
    {
        this->transform = transform;
        this->matrix = new Matrix();
    }

    VectorHomo GetPosition()
    {
        return transform;
    }

    vector<VectorHomo> GetHitbox()
    {
        vector<VectorHomo> updatedHitbox;

        for (auto point : hitbox)
        {
            updatedHitbox.push_back(this->transform + point);
        }

        return updatedHitbox;
    }

    virtual void Update(){}
protected:
    virtual void Render(){}

    float moveSpeed = 0;

    VectorHomo transform;
    Matrix* matrix = NULL;
    vector<VectorHomo> hitbox;

    void Move(VectorHomo inc)
    {
        matrix->BuildIdentity();
        matrix->Translation(inc.x * FpsController::getInstance().normalize(moveSpeed),
                            inc.y * FpsController::getInstance().normalize(moveSpeed));
        this->transform = *matrix * this->transform;
        matrix->Reset();
    }
};

#endif // ENTITY_H_INCLUDED
