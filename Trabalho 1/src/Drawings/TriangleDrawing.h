#ifndef TRIANGLEDRAWING_H_INCLUDED
#define TRIANGLEDRAWING_H_INCLUDED

#include "Drawing.h"

class TriangleDrawing : public Drawing
{
    public:
    TriangleDrawing(int x, int y, int width, int height)
    {
        this->type = Triangle;

        this->elementsCounter = 3;

        this->xs = new float[this->elementsCounter];
        this->ys = new float[this->elementsCounter];

        this->AddPoint(x, y + height, 0);
        this->AddPoint(x + width / 2, y, 1);
        this->AddPoint(x + width, y + height, 2);
    }
};

#endif // TRIANGLEDRAWING_H_INCLUDED
