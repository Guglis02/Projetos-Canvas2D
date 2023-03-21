#include "Obstacle.h"
#include "gl_canvas2d.h"

Obstacle::Obstacle(float x, float y, float gapTopPosition)
{
    this->x = x;
    this->y = y;
    this->gapTopPosition = gapTopPosition;
    this->gapBottonPosition = gapTopPosition + gapHeight;
}

void Obstacle::Update(float deltaTime)
{
    Move(deltaTime);
    Draw();
}

void Obstacle::Move(float deltaTime)
{
    x = x - (moveSpeed * deltaTime);

    printf("\n speed:%f x:%f", moveSpeed, x);
}

void Obstacle::Draw()
{
    color(3);
    rectFill(x, 0, x + width, gapTopPosition);
    rectFill(x, gapBottonPosition, x + width, y);
}

void Obstacle::Reset()
{
    x = 500 + width;
}
