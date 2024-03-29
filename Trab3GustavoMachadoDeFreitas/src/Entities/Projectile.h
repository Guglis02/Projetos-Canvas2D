#ifndef PROJECTILE_H_INCLUDED
#define PROJECTILE_H_INCLUDED

#include "../gl_canvas2d.h"
#include "Entity.h"

// Classe que representa um projétil
class Projectile : public Entity
{
public:
    Projectile(VectorHomo transform) : Entity(transform)
    {
        this->moveSpeed = 1000;

        this->hitbox.push_back(VectorHomo(-5, 0));
        this->hitbox.push_back(VectorHomo(-5, 25));
        this->hitbox.push_back(VectorHomo(5, 25));
        this->hitbox.push_back(VectorHomo(5, 0));
    }
    ~Projectile() {}

    void Update()
    {
        Move(moveDirection);

        Render();
    }

protected:
    int colorIndex;
    VectorHomo moveDirection;

    void Render()
    {
        CV::color(colorIndex);
        CV::rectFill(transform.x - 5, transform.y, transform.x + 5, transform.y + 25);
    }
};

// Método utilitário para saber de que tipo é um projétil
template <typename T>
static bool IsOfType(Projectile *projectile)
{
    return dynamic_cast<T *>(projectile) != nullptr;
}

#endif // PROJECTILE_H_INCLUDED
