#include "Player.h"
#include "gl_canvas2d.h"

Player::Player(float x, float y)
{
    this->x = x;
    this->y = y;
};

void Player::Draw()
{
    color(2);
    circleFill( x, y, 30, 20);
};

void Player::UpdateSpeedAndPosition(float deltaTime)
{
    this->fallSpeed = fallSpeed + acceleration * deltaTime;

    if(fallSpeed > flapForce) {fallSpeed = flapForce;}

    y = y + fallSpeed * deltaTime;

    if (y < 0) {y = 0;}
    if (y > 500) {y = 500;}

    color(2);
    text(500 / 2 - 50, 500 / 3, fallSpeed);
};

void Player::Flap()
{
    fallSpeed = -flapForce;
};
