#ifndef POWERUP_H_INCLUDED
#define POWERUP_H_INCLUDED

#include "../gl_canvas2d.h"
#include "Entity.h"
#include <functional>

// Classe que representa um projétil
class PowerUp : public Entity
{
public:
    PowerUp(VectorHomo transform, char* symbol, function<void()> buffCallback) : Entity(transform)
    {
        this->moveSpeed = 600;
        this->moveDirection = VectorHomo(0, -1);
        this->symbol = symbol;
        this->buffCallback = buffCallback;

        this->hitbox.push_back(VectorHomo(-16, 16));
        this->hitbox.push_back(VectorHomo(16, 16));
        this->hitbox.push_back(VectorHomo(16, -16));
        this->hitbox.push_back(VectorHomo(-16, -16));
    }

    void Update()
    {
        Move(moveDirection);

        Render();
    }

    bool CheckCollision(vector<VectorHomo> hitbox)
    {
        for (auto point : GetHitbox())
        {
            if (pnpoly(hitbox.size(), hitbox.data(), point))
            {
                return true;
            }
        }

        return false;
    }
    
    function<void()> buffCallback;

protected:
    int colorIndex;
    VectorHomo moveDirection;
    char* symbol;

    void Render()
    {
        CV::color(0, 0, 0.6);
        CV::circleFill(transform, 16, 16);
        CV::color(1, 1, 1);
        CV::text(transform, symbol);
    }
};

#endif // POWERUP_H_INCLUDED
