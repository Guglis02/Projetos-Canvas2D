#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include "../Utils/VectorHomo.h"
#include "../Utils/Matrix.h"
#include "../Utils/FpsController.h"
#include "../Utils/PointsUtils.h"
#include <vector>

using namespace std;

// Classe base para todas as entidades do jogo
class Entity
{
public:
    Entity(VectorHomo transform)
    {
        this->transform = transform;
        this->matrix = new Matrix();
    }
    virtual ~Entity() {}

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

    bool CheckCollision(vector<VectorHomo> hitbox)
    {
        for (auto point : hitbox)
        {
            if (pnpoly(GetHitbox().size(), GetHitbox().data(), point))
            {
                return true;
            }
        }

        return false;
    }

    void OnHit()
    {
        delete this;
    }

    virtual void Update() {}

protected:
    virtual void Render() {}

    float moveSpeed = 0;

    VectorHomo transform;
    Matrix *matrix = NULL;
    vector<VectorHomo> hitbox;

    // Recebe um vetor direção e o multiplica pela velocidade de movimento normalizada pelo FPS
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
