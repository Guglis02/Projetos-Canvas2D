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

            this->centerX = x;
            this->centerY = y;

            this->width = radius * 2;
            this->height = radius * 2;

            this->elementsCounter = div;

            float ang = 0;
            float inc = PI_2/div;

            for(int i = 0; i < div; i++)
            {
                this->AddPoint(x + (cos(ang)*radius), y + (sin(ang)*radius), i);
                ang+=inc;
            }

            this->AddSelectionPoint(x - radius, y - radius, 0);
            this->AddSelectionPoint(x + radius, y - radius, 1);
            this->AddSelectionPoint(x + radius, y + radius, 2);
            this->AddSelectionPoint(x - radius, y + radius, 3);
        }
};

#endif // CIRCLEDRAWING_H_INCLUDED
