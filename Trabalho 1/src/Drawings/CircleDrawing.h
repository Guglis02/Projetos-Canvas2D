#ifndef CIRCLEDRAWING_H_INCLUDED
#define CIRCLEDRAWING_H_INCLUDED

#include "Drawing.h"

class CircleDrawing : public Drawing
{
    public:
    CircleDrawing(int x, int y, int radius, int div)
    {
        this->type = Circle;

        this->xs = new float[div];
        this->ys = new float[div];

        this->elementsCounter = div;

        float ang = 0;
        float inc = PI_2/div;

        for(int i = 0; i < div; i++)
        {
            this->AddPoint(x + (cos(ang)*radius), y + (sin(ang)*radius), i);
            ang+=inc;
        }
    }
};

#endif // CIRCLEDRAWING_H_INCLUDED
