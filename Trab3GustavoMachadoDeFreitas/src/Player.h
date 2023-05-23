#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "gl_canvas2d.h"
#include "Vector2.h"
#include "Entity.h"

class Player : public Entity
{
public:
    Player(Vector2 transform)
        : Entity(transform)
    {
        this->moveSpeed = 1;
        this->movementDirection = Vector2(0, 0);
    }

    void HandleMovement()
    {
        if (isMovingUp)
        {
            movementDirection += Vector2(0, 1);
        }
        if (isMovingDown)
        {
            movementDirection += Vector2(0, -1);
        }
        if (isMovingLeft)
        {
            movementDirection += Vector2(-1, 0);
        }
        if (isMovingRight)
        {
            movementDirection += Vector2(1, 0);
        }

        if (movementDirection == Vector2(0, 0))
        {
            return;
        }

        movementDirection.normalize();

        Move(movementDirection);
    }


    void Update()
    {
        HandleMovement();

        Render();
    }

    bool isMovingUp = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;
    bool isMovingDown = false;

    Vector2 movementDirection;

protected:
    void Render()
    {
        CV::translate(transform);

        CV::color(0.5f, 0.5f, 0.5f);

        float lx[] = {-32, -20, -5, -5};
        float ly[] = {5, 50, 50, 5};
        CV::polygonFill(lx, ly, 4);

        float rx[] = {32, 20, 5, 5};
        float ry[] = {5, 50, 50, 5};
        CV::polygonFill(rx, ry, 4);

        CV::color(0.6f, 0.6f, 0.6f);
        CV::circleFill(0, 0, 32, 32);

        CV::color(0.7f, 0.7f, 0.7f);
        CV::circleFill(0, 0, 10, 10);
        CV::rectFill(-32, 5, 32, -5);

        CV::rectFill(32, 32, 40, 20);
        float bx[] = {0, 32, 40, 5};
        float by[] = {0, 20, 20, -5};
        CV::polygonFill(bx, by, 4);

        CV::color(0, 0, 0);
        CV::circle(0, 0, 10, 10);

        CV::translate(0, 0);
    }
};

#endif // PLAYER_H_INCLUDED
