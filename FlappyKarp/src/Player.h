#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <functional>
#include "gl_canvas2d.h"

class Player
{
    public:
        /*Construtor*/
        Player(float x, float y);

        void Draw();
        void UpdateSpeedAndPosition(float deltaTime);
        void Flap();
    private:
        const float acceleration = 0.001;
        const float flapForce = 0.3;

        float x;
        float y;
        float fallSpeed = 0.01;
};



#endif // PLAYER_H_INCLUDED
